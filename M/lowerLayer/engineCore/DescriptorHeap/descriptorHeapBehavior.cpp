#include "descriptorHeapBehavior.h"
#include <assert.h>
#pragma comment(lib,"d3d12.lib")

SIZE_T DescriptorHeapBehavior::GetCPUDescriptorHandle(uint16_t index_)
{
	//最大検知
	if (index_ > kMaxDescriptor) assert(false);

	D3D12_CPU_DESCRIPTOR_HANDLE ret_handleCPU = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
	ret_handleCPU.ptr += sizeOfDescriptor * index_;
	

	return ret_handleCPU.ptr;
}

SIZE_T DescriptorHeapBehavior::GetGPUDescriptorHandle(uint16_t index_)
{
	//最大検知
	if (index_ > kMaxDescriptor) assert(false);

	D3D12_GPU_DESCRIPTOR_HANDLE ret_handleGPU = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
	ret_handleGPU.ptr += sizeOfDescriptor * index_;

	return ret_handleGPU.ptr;
}

D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHeapBehavior::GetCPUDescriptorHandle2(uint16_t index_)
{
	D3D12_CPU_DESCRIPTOR_HANDLE ret_handleCPU = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
	ret_handleCPU.ptr += sizeOfDescriptor * index_;

	return ret_handleCPU;
}

D3D12_GPU_DESCRIPTOR_HANDLE DescriptorHeapBehavior::GetGPUDescriptorHandle2(uint16_t index_)
{
	D3D12_GPU_DESCRIPTOR_HANDLE ret_handleGPU = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
	ret_handleGPU.ptr += sizeOfDescriptor * index_;

	return ret_handleGPU;
}


Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> DescriptorHeapBehavior::CreateDescriptorHeap(ID3D12Device* device_, D3D12_DESCRIPTOR_HEAP_TYPE heapType_, UINT numDescriptors_, bool shaderVisible)
{
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> ret_descriptorHeap = nullptr;
	D3D12_DESCRIPTOR_HEAP_DESC descriptorheapDesc{};

	descriptorheapDesc.NodeMask = 0;
	descriptorheapDesc.Type = heapType_;
	descriptorheapDesc.NumDescriptors = numDescriptors_;
	descriptorheapDesc.Flags = shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	HRESULT hr = device_->CreateDescriptorHeap(&descriptorheapDesc,
		IID_PPV_ARGS(ret_descriptorHeap.GetAddressOf()));

	assert(SUCCEEDED(hr));

	return ret_descriptorHeap;
}

