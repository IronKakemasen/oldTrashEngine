#include "dsvDescriptorHeap.h"

void DsvDescriptorHeap::Init(ID3D12Device* device_, uint16_t numDescriptors_)
{
	descriptorHeap = DescriptorHeapBehavior::CreateDescriptorHeap(device_, 
		D3D12_DESCRIPTOR_HEAP_TYPE_DSV, numDescriptors_, false);
	kMaxDescriptor = numDescriptors_;
	sizeOfDescriptor = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

}