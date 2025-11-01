module;
#include "windows.h"
export module native_window;

import std;

// for intellisense

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

export class NativeWindow
{
public:
	static std::unordered_map<HWND, NativeWindow*> windows;

	explicit NativeWindow()
	{
		HICON hIconLarge = (HICON)LoadImage(nullptr, L"icon.ico", IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
		HICON hIconSmall = (HICON)LoadImage(nullptr, L"icon.ico", IMAGE_ICON, 16, 16, LR_LOADFROMFILE);

		//ImGui_ImplWin32_EnableDpiAwareness();
		wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), hIconLarge, nullptr, nullptr, nullptr, L"Giraud", hIconSmall };
		RegisterClassExW(&wc);
		hwnd = CreateWindowW(wc.lpszClassName, L"Giraud", WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, nullptr, nullptr, wc.hInstance, nullptr);

		// Show the window
		ShowWindow(hwnd, SW_SHOWDEFAULT);
		UpdateWindow(hwnd);

		windows[hwnd] = this;
	}

	~NativeWindow()
	{
		windows.erase(hwnd);
		DestroyWindow(hwnd);
		UnregisterClassW(wc.lpszClassName, wc.hInstance);
	}

	bool ProcessEvents()
	{
		MSG msg;
		while (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if (msg.message == WM_QUIT)
			{
				return false;
			}
		}
		return true;
	}

	bool ProcessMessage(UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (OnMessage)
		{
			OnMessage(msg, wParam, lParam);
		}

		switch (msg)
		{
		case WM_SIZE:
			if (wParam != SIZE_MINIMIZED && OnResize)
			{
				OnResize((UINT)LOWORD(lParam), (UINT)HIWORD(lParam));
			}
			return true;
		case WM_SYSCOMMAND:
			if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			{
				return true;
			}
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			return true;
		}

		return false;
	}

	std::function<void(UINT, WPARAM, LPARAM)> OnMessage;
	std::function<void(UINT, UINT)> OnResize;

	WNDCLASSEXW wc;
	HWND hwnd;
};

LRESULT WINAPI WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	auto window = NativeWindow::windows[hwnd];
	if (window)
	{
		if (window->ProcessMessage(msg, wParam, lParam))
		{
			return 0;
		}
	}

	return DefWindowProcW(hwnd, msg, wParam, lParam);
}

std::unordered_map<HWND, NativeWindow*> NativeWindow::windows;