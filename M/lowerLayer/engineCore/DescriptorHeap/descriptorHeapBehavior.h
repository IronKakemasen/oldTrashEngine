#pragma once

#include <wrl.h>
#include <d3d12.h>
#include <stdint.h>

class DescriptorHeapBehavior
{

protected:
	//[ DescriptorHeap ]
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap = nullptr;

	//[ DescriptorSize ]
	uint32_t sizeOfDescriptor = 0;

	//最大領域分割数
	uint16_t kMaxDescriptor = 0;

public:

	inline auto* Getter_Descriptorheap()
	{
		return descriptorHeap.Get();
	}

	inline auto Getter_MaxDescriptor()
	{
		return kMaxDescriptor;
	}


	virtual void Init(ID3D12Device* device_, uint16_t numDescriptors_) = 0;

	[[nodiscard]] SIZE_T GetCPUDescriptorHandle(uint16_t index_);
	[[nodiscard]] SIZE_T GetGPUDescriptorHandle(uint16_t index_);

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle2(uint16_t index_);
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle2(uint16_t index_);


	static [[nodiscard]] Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap(ID3D12Device* device_,
		D3D12_DESCRIPTOR_HEAP_TYPE heapType_, UINT numDescriptors_, bool shaderVisible);

};


