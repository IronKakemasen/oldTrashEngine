#pragma once

#include <wrl.h>
#include <d3d12.h>
#include <vector>

class CommandControl
{
	//コマンドキュー
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue = nullptr;
	//コマンドアローケータ
	std::vector<Microsoft::WRL::ComPtr<ID3D12CommandAllocator>> commandAllocators = { nullptr };
	//コマンドリスト		
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList6> commandList = nullptr;

public:

	inline ID3D12CommandQueue* Getter_CommandQueue()
	{
		return commandQueue.Get();
	}

	inline ID3D12CommandAllocator* Getter_CommandAllocator(uint8_t index_)
	{
		return commandAllocators[index_].Get();
	}

	inline ID3D12GraphicsCommandList6* Getter_commandList()
	{
		return commandList.Get();
	}

	void Initialize();




	//コマンドキューの生成
	void MakeCommandQueue(ID3D12Device* device_);
	//コマンドアローケータの生成
	void MakeCommandAllocator(ID3D12Device* device_);
	//コマンドリストを生成する
	void MakeCommandList(ID3D12Device* device_);

	void PrepareForNextCommandList();


};

