#include "srvDescriptorHeap.h"

void SrvDescriptorHeap::Init(ID3D12Device* device_, uint16_t numDescriptors_)
{
	descriptorHeap = DescriptorHeapBehavior::CreateDescriptorHeap(device_, 
		D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, numDescriptors_, true);
	kMaxDescriptor = numDescriptors_;
	sizeOfDescriptor = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

}