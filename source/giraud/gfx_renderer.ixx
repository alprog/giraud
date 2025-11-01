module;
#include <d3d12.h>
#include <dxgi1_4.h>
#include <tchar.h>
#ifdef _DEBUG
#define DX12_ENABLE_DEBUG_LAYER
#endif

#ifdef DX12_ENABLE_DEBUG_LAYER
#include <dxgidebug.h>
#pragma comment(lib, "dxguid.lib")
#endif
#include "assert.h"
export module gfx_renderer;

import native_window;

// for intellisense

export constexpr const int NUM_FRAMES_IN_FLIGHT = 2;
export constexpr const int NUM_BACK_BUFFERS = 2;
export constexpr const int SRV_HEAP_SIZE = 64;

export class GfxRenderer;

export struct FrameContext
{
	ID3D12CommandAllocator* CommandAllocator;
	UINT64 FenceValue;
	UINT BackBufferIndex;
};

export class GfxRenderer
{
public:
	FrameContext frameContext[NUM_FRAMES_IN_FLIGHT] = {};
	UINT frameIndex = 0;
	ID3D12Device* pd3dDevice = nullptr;
	ID3D12DescriptorHeap* pd3dRtvDescHeap = nullptr;
	ID3D12DescriptorHeap* pd3dSrvDescHeap = nullptr;
	ID3D12CommandQueue* pd3dCommandQueue = nullptr;
	ID3D12GraphicsCommandList* pd3dCommandList = nullptr;
	ID3D12Fence* fence = nullptr;
	HANDLE fenceEvent = nullptr;
	UINT64 fenceLastSignaledValue = 0;
	IDXGISwapChain3* pSwapChain = nullptr;
	bool SwapChainOccluded = false;
	HANDLE hSwapChainWaitableObject = nullptr;
	ID3D12Resource* mainRenderTargetResource[NUM_BACK_BUFFERS] = {};
	D3D12_CPU_DESCRIPTOR_HANDLE mainRenderTargetDescriptor[NUM_BACK_BUFFERS] = {};
	float clearColor[4] = { 0.45f, 0.55f, 0.60f, 1.00f };

	explicit GfxRenderer(NativeWindow& window)
	{
		CreateDeviceD3D(window.hwnd);

		window.OnResize = [this](UINT width, UINT height) {
			WaitForLastSubmittedFrame();
			CleanupRenderTarget();
			HRESULT result = pSwapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT);
			assert(SUCCEEDED(result) && "Failed to resize swapchain.");
			CreateRenderTarget();
			};
	}

	~GfxRenderer()
	{
		CleanupDeviceD3D();
	}

	bool CreateDeviceD3D(HWND hWnd)
	{
		// Setup swap chain
		DXGI_SWAP_CHAIN_DESC1 sd;
		{
			ZeroMemory(&sd, sizeof(sd));
			sd.BufferCount = NUM_BACK_BUFFERS;
			sd.Width = 0;
			sd.Height = 0;
			sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			sd.Flags = DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT;
			sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			sd.SampleDesc.Count = 1;
			sd.SampleDesc.Quality = 0;
			sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
			sd.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
			sd.Scaling = DXGI_SCALING_STRETCH;
			sd.Stereo = FALSE;
		}

		// [DEBUG] Enable debug interface
#ifdef DX12_ENABLE_DEBUG_LAYER
		ID3D12Debug* pdx12Debug = nullptr;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&pdx12Debug))))
			pdx12Debug->EnableDebugLayer();
#endif

		// Create device
		D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
		if (D3D12CreateDevice(nullptr, featureLevel, IID_PPV_ARGS(&pd3dDevice)) != S_OK)
			return false;

		// [DEBUG] Setup debug interface to break on any warnings/errors
#ifdef DX12_ENABLE_DEBUG_LAYER
		if (pdx12Debug != nullptr)
		{
			ID3D12InfoQueue* pInfoQueue = nullptr;
			pd3dDevice->QueryInterface(IID_PPV_ARGS(&pInfoQueue));
			pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
			pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
			pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);
			pInfoQueue->Release();
			pdx12Debug->Release();
		}
#endif

		{
			D3D12_DESCRIPTOR_HEAP_DESC desc = {};
			desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
			desc.NumDescriptors = NUM_BACK_BUFFERS;
			desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			desc.NodeMask = 1;
			if (pd3dDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&pd3dRtvDescHeap)) != S_OK)
				return false;

			SIZE_T rtvDescriptorSize = pd3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
			D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = pd3dRtvDescHeap->GetCPUDescriptorHandleForHeapStart();
			for (UINT i = 0; i < NUM_BACK_BUFFERS; i++)
			{
				mainRenderTargetDescriptor[i] = rtvHandle;
				rtvHandle.ptr += rtvDescriptorSize;
			}
		}

		{
			D3D12_DESCRIPTOR_HEAP_DESC desc = {};
			desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
			desc.NumDescriptors = SRV_HEAP_SIZE;
			desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
			if (pd3dDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&pd3dSrvDescHeap)) != S_OK)
				return false;
		}

		{
			D3D12_COMMAND_QUEUE_DESC desc = {};
			desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
			desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
			desc.NodeMask = 1;
			if (pd3dDevice->CreateCommandQueue(&desc, IID_PPV_ARGS(&pd3dCommandQueue)) != S_OK)
				return false;
		}

		for (UINT i = 0; i < NUM_FRAMES_IN_FLIGHT; i++)
			if (pd3dDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&frameContext[i].CommandAllocator)) != S_OK)
				return false;

		if (pd3dDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, frameContext[0].CommandAllocator, nullptr, IID_PPV_ARGS(&pd3dCommandList)) != S_OK ||
			pd3dCommandList->Close() != S_OK)
			return false;

		if (pd3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence)) != S_OK)
			return false;

		fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		if (fenceEvent == nullptr)
			return false;

		{
			IDXGIFactory4* dxgiFactory = nullptr;
			IDXGISwapChain1* swapChain1 = nullptr;
			if (CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory)) != S_OK)
				return false;
			if (dxgiFactory->CreateSwapChainForHwnd(pd3dCommandQueue, hWnd, &sd, nullptr, nullptr, &swapChain1) != S_OK)
				return false;
			if (swapChain1->QueryInterface(IID_PPV_ARGS(&pSwapChain)) != S_OK)
				return false;
			swapChain1->Release();
			dxgiFactory->Release();
			pSwapChain->SetMaximumFrameLatency(NUM_BACK_BUFFERS);
			hSwapChainWaitableObject = pSwapChain->GetFrameLatencyWaitableObject();
		}

		CreateRenderTarget();
		return true;
	}

	void CreateRenderTarget()
	{
		for (UINT i = 0; i < NUM_BACK_BUFFERS; i++)
		{
			ID3D12Resource* pBackBuffer = nullptr;
			pSwapChain->GetBuffer(i, IID_PPV_ARGS(&pBackBuffer));
			pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, mainRenderTargetDescriptor[i]);
			mainRenderTargetResource[i] = pBackBuffer;
		}
	}

	void CleanupDeviceD3D()
	{
		CleanupRenderTarget();
		if (pSwapChain) { pSwapChain->SetFullscreenState(false, nullptr); pSwapChain->Release(); pSwapChain = nullptr; }
		if (hSwapChainWaitableObject != nullptr) { CloseHandle(hSwapChainWaitableObject); }
		for (UINT i = 0; i < NUM_FRAMES_IN_FLIGHT; i++)
			if (frameContext[i].CommandAllocator) { frameContext[i].CommandAllocator->Release(); frameContext[i].CommandAllocator = nullptr; }
		if (pd3dCommandQueue) { pd3dCommandQueue->Release(); pd3dCommandQueue = nullptr; }
		if (pd3dCommandList) { pd3dCommandList->Release(); pd3dCommandList = nullptr; }
		if (pd3dRtvDescHeap) { pd3dRtvDescHeap->Release(); pd3dRtvDescHeap = nullptr; }
		if (pd3dSrvDescHeap) { pd3dSrvDescHeap->Release(); pd3dSrvDescHeap = nullptr; }
		if (fence) { fence->Release(); fence = nullptr; }
		if (fenceEvent) { CloseHandle(fenceEvent); fenceEvent = nullptr; }
		if (pd3dDevice) { pd3dDevice->Release(); pd3dDevice = nullptr; }

#ifdef DX12_ENABLE_DEBUG_LAYER
		IDXGIDebug1* pDebug = nullptr;
		if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&pDebug))))
		{
			pDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_SUMMARY);
			pDebug->Release();
		}
#endif
	}

	void CleanupRenderTarget()
	{
		WaitForLastSubmittedFrame();

		for (UINT i = 0; i < NUM_BACK_BUFFERS; i++)
			if (mainRenderTargetResource[i]) { mainRenderTargetResource[i]->Release(); mainRenderTargetResource[i] = nullptr; }
	}

	FrameContext& GetCurrentFrameContext()
	{
		return frameContext[frameIndex % NUM_FRAMES_IN_FLIGHT];
	}

	void WaitForLastSubmittedFrame()
	{
		FrameContext& frameCtx = GetCurrentFrameContext();

		UINT64 fenceValue = frameCtx.FenceValue;
		if (fenceValue == 0)
			return; // No fence was signaled

		frameCtx.FenceValue = 0;
		if (fence->GetCompletedValue() >= fenceValue)
			return;

		fence->SetEventOnCompletion(fenceValue, fenceEvent);
		WaitForSingleObject(fenceEvent, INFINITE);
	}

	FrameContext& WaitForCurrentFrameResources()
	{
		HANDLE waitableObjects[] = { hSwapChainWaitableObject, nullptr };
		DWORD numWaitableObjects = 1;

		FrameContext& frameCtx = GetCurrentFrameContext();
		UINT64 fenceValue = frameCtx.FenceValue;
		if (fenceValue != 0) // means no fence was signaled
		{
			frameCtx.FenceValue = 0;
			fence->SetEventOnCompletion(fenceValue, fenceEvent);
			waitableObjects[1] = fenceEvent;
			numWaitableObjects = 2;
		}

		WaitForMultipleObjects(numWaitableObjects, waitableObjects, TRUE, INFINITE);
		return frameCtx;
	}

	void SetBarrier(int backBufferIdx, D3D12_RESOURCE_STATES Before, D3D12_RESOURCE_STATES After)
	{
		D3D12_RESOURCE_BARRIER barrier = {};
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barrier.Transition.pResource = mainRenderTargetResource[backBufferIdx];
		barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		barrier.Transition.StateBefore = Before;
		barrier.Transition.StateAfter = After;
		pd3dCommandList->ResourceBarrier(1, &barrier);
	}

	void StartFrame()
	{
		frameIndex++;

		FrameContext& frameCtx = WaitForCurrentFrameResources();
		frameCtx.BackBufferIndex = pSwapChain->GetCurrentBackBufferIndex();
		frameCtx.CommandAllocator->Reset();
		pd3dCommandList->Reset(frameCtx.CommandAllocator, nullptr);
		SetBarrier(frameCtx.BackBufferIndex, D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
		pd3dCommandList->ClearRenderTargetView(mainRenderTargetDescriptor[frameCtx.BackBufferIndex], clearColor, 0, nullptr);
		pd3dCommandList->OMSetRenderTargets(1, &mainRenderTargetDescriptor[frameCtx.BackBufferIndex], FALSE, nullptr);
		pd3dCommandList->SetDescriptorHeaps(1, &pd3dSrvDescHeap);
	}

	void EndFrame()
	{
		SetBarrier(GetCurrentFrameContext().BackBufferIndex, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
		pd3dCommandList->Close();
		pd3dCommandQueue->ExecuteCommandLists(1, (ID3D12CommandList* const*)&pd3dCommandList);
	}

	void Present()
	{
		HRESULT hr = pSwapChain->Present(1, 0);   // Present with vsync
		//HRESULT hr = g_pSwapChain->Present(0, 0); // Present without vsync
		SwapChainOccluded = (hr == DXGI_STATUS_OCCLUDED);

		UINT64 fenceValue = fenceLastSignaledValue + 1;
		pd3dCommandQueue->Signal(fence, fenceValue);
		fenceLastSignaledValue = fenceValue;
		GetCurrentFrameContext().FenceValue = fenceValue;
	}
};
