#pragma once

#include <wrl.h>
#include <d3d12.h>
#include <stdint.h>
#include <dxgi1_6.h>
#include "../SwapChainControl/SwapChainControl.h"

class FenceControl
{
	//[ Fence ]
	Microsoft::WRL::ComPtr<ID3D12Fence> fence = nullptr;
	uint64_t fenceCounter[SwapChainControl::kFrameBufferCnt] = {0,0};
	HANDLE fenceEvent = nullptr;


public:
	inline HANDLE Getter_FenceEvent()
	{
		return fenceEvent;
	}

	void SetFence(ID3D12Device* device_, IDXGIFactory7* dxgiFactory_);
	void Initialize(ID3D12Device* device_, IDXGIFactory7* dxgiFactory_);
	void WaitFenceEvent(ID3D12CommandQueue* commandQueue_, IDXGISwapChain4* swapChain_);

};

