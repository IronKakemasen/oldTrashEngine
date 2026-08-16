#pragma once

#include "../../Buffer/gpuResources/Resource/colorBuffer/colorBuffer.h"
#include "../../Buffer/gpuResources/Resource/depthBuffer/depthBuffer.h"
#include <dxgi1_6.h>

class SwapChainControl
{
public:

	static inline const UINT kFrameBufferCnt  = 2;
	static inline UINT frameIndex = 0;


private:

	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain = nullptr;
	ColorBuffer colorBuffers[kFrameBufferCnt];
	DepthBuffer depthBuffer;

	//[dear ImGui]
	DXGI_SWAP_CHAIN_DESC1 swapChainDescForImGui;
	D3D12_RENDER_TARGET_VIEW_DESC renderTargetDescForImGui;

	void CreateRenderTargetView(ID3D12Device* device_, RtvDescriptorHeap* rtvDescHeap_);
	void CreateDepthSencilView(ID3D12Device* device_, DsvDescriptorHeap* dsvDescHeap_);

	//swapChainの生成
	DXGI_SWAP_CHAIN_DESC1 CreateSwapChain(HWND* hwnd_, ID3D12CommandQueue* commandQueue_,
		IDXGIFactory7* dxgiFactory_);

	//SwapChainからリソースを引っ張る
	void PullSwapChainResource();


public:

	inline auto* Getter_SwapChain()
	{
		return swapChain.Get();
	}

	inline auto* Getter_DepthBuffer()
	{
		return &depthBuffer;
	}


	inline auto* Getter_ColorBuffer(uint8_t index_)
	{
		return &colorBuffers[index_];
	}

	inline auto Ref_SwapChainDesc()
	{
		return swapChainDescForImGui;
	}

	inline auto Ref_RenderTargetDesc()
	{
		return renderTargetDescForImGui;
	}


	void Initialize(HWND* hwnd_, ID3D12CommandQueue* commandQueue_,
		IDXGIFactory7* dxgiFactory_, ID3D12Device* device_, 
		RtvDescriptorHeap* rtvDescHeap_, DsvDescriptorHeap* dsvDescHeap_);



};


