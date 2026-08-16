#pragma once
#include <wrl.h>
#include <d3d12.h>
#include<stdint.h>
#include "../SrvCreatorBehavior.h"


struct ShaderBufferData;
struct SrvDescriptorHeap;

class ParticleMeshSrvCreator:public SrvCreatorBehavior
{
	D3D12_SHADER_RESOURCE_VIEW_DESC CreateSRVDesc(UINT maxParticle_);

public:
	uint16_t CreateSRVForParticle(UINT maxParticle_, ShaderBuffer& data_);
};

