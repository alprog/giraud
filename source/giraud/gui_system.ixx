module;
#include "imgui.h"
#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_dx12.h"
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
export module gui_system;

import std;
import native_window;
import gfx_renderer;
import gui_panel;

// for intellisense

export class GuiSystem
{
public:
	explicit GuiSystem(NativeWindow& nativeWindow, GfxRenderer& gfxRenderer)
		: nativeWindow{ nativeWindow }
		, gfxRenderer{ gfxRenderer }
	{
		InitializeConfig();
		InitializeBackends();
	}

	~GuiSystem()
	{
		ImGui_ImplDX12_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}

	template <typename TPanel, typename... TArgs>
	void AddPanel(TArgs&&... Args)
	{
		panels.emplace_back(std::make_unique<TPanel>(std::forward<TArgs>(Args)...));
	}

	void ShowDemoPanel()
	{
		isDemoPanelShown = true;
	}

	void NewFrame()
	{
		ImGui_ImplDX12_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());
	}

	void PrepareDraw()
	{
		for (auto& panel : panels)
		{
			panel->Render();
		}

		if (isDemoPanelShown)
		{
			ImGui::ShowDemoWindow(&isDemoPanelShown);
		}

		ImGui::Render();
	}

	void Draw()
	{
		ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), gfxRenderer.pd3dCommandList);
	}

	void RenderPlatformWindows()
	{
		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	}

private:
	void InitializeConfig()
	{
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
		//io.ConfigViewportsNoAutoMerge = true;
		//io.ConfigViewportsNoTaskBarIcon = true;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsLight();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}
	}

	void InitializeBackends()
	{
		ImGui_ImplWin32_Init(nativeWindow.hwnd);

		nativeWindow.OnMessage = [this](UINT msg, WPARAM wParam, LPARAM lParam) {
			ImGui_ImplWin32_WndProcHandler(nativeWindow.hwnd, msg, wParam, lParam);
			};

		ImGui_ImplDX12_Init(
			gfxRenderer.pd3dDevice,
			NUM_FRAMES_IN_FLIGHT,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			gfxRenderer.pd3dSrvDescHeap,
			gfxRenderer.pd3dSrvDescHeap->GetCPUDescriptorHandleForHeapStart(),
			gfxRenderer.pd3dSrvDescHeap->GetGPUDescriptorHandleForHeapStart()
		);
	}

	NativeWindow& nativeWindow;
	GfxRenderer& gfxRenderer;
	std::vector<std::unique_ptr<GuiPanel>> panels;
	bool isDemoPanelShown = true;
};