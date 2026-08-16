#pragma once
#include "../descriptorHeapBehavior.h"

struct RtvDescriptorHeap:DescriptorHeapBehavior
{	
	virtual void Init(ID3D12Device* device_, uint16_t numDescriptors_)override;
};

