#pragma once
#include <wrl.h>
#include <d3d12.h>
#include<stdint.h>
#include "../SrvCreatorBehavior.h"


struct ShaderBufferData;
struct SrvDescriptorHeap;

class StructuredBufferSrvCreator :public SrvCreatorBehavior
{
	D3D12_SHADER_RESOURCE_VIEW_DESC CreateSRVDesc(UINT createNum_,UINT size_);

public:
	uint16_t CreateSRVForStructuredBuffer(UINT createNum_, UINT size_, ShaderBuffer& data_);
};
