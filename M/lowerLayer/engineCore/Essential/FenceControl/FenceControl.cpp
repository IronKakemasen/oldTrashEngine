#include "FenceControl.h"
#include <assert.h>
#include "../../WinApp.h"


#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

void FenceControl::WaitFenceEvent(ID3D12CommandQueue* commandQueue_, IDXGISwapChain4* swapChain_)
{
	////Fenceの値を更新
	//fenceCounter[SwapChainControl::frameIndex]++;
	
	//GPUがここまでたどり着いたとき、
	//fenceの値を指定した値に代入するようにSignalを送る
	const auto currentValue = fenceCounter[SwapChainControl::frameIndex];

	commandQueue_->Signal(fence.Get(), currentValue);

	// バックバッファ番号を更新.
	SwapChainControl::frameIndex = swapChain_->GetCurrentBackBufferIndex();


	//fenceの値が指定したSignal値にたどり着いているか確認する
	//GetCompletedValueの初期値はfence作成時に渡した初期値
	if (fence->GetCompletedValue() < fenceCounter[SwapChainControl::frameIndex])
	{
		//指定したSignalにたどり着いていないので、たどり着くまで待つ
		fence->SetEventOnCompletion(fenceCounter[SwapChainControl::frameIndex], fenceEvent);
		//イベント待つ
		WaitForSingleObjectEx(fenceEvent, INFINITE,FALSE);
	}

	// 次のフレームのフェンスカウンターを増やす.
	fenceCounter[SwapChainControl::frameIndex] = currentValue + 1;

}


void FenceControl::Initialize(ID3D12Device* device_, IDXGIFactory7* dxgiFactory_)
{
	SetFence(device_, dxgiFactory_);
}

void FenceControl::SetFence(ID3D12Device* device_, IDXGIFactory7* dxgiFactory_)
{

	HRESULT hr = device_->CreateFence(
		fenceCounter[SwapChainControl::frameIndex],
		D3D12_FENCE_FLAG_NONE,
		IID_PPV_ARGS(fence.GetAddressOf()));

	assert(SUCCEEDED(hr));
	Log(WinApp::log, "Complete built fence\n");

	fenceCounter[SwapChainControl::frameIndex]++;

	//FenceのSignalを待つためのイベント（WINDOWへのメッセージ）を作成する
	fenceEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	assert(fenceEvent != nullptr);
	Log(WinApp::log, "Complete creat event\n");

}
