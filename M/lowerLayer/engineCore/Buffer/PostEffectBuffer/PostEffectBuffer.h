#pragma once
#include "../gpuResources/Resource/shaderBuffer/shaderBuffer.h"
#include "../gpuResources/Resource/colorBuffer/colorBuffer.h"
#include "../gpuResourcesBehavior.h"
#include <assert.h>
#include <memory>
#include "../gpuResources/Resource/depthBuffer/depthBuffer.h"
#include "../../PSO/pipelineCreators/pipelineComponents.h"

struct DsvDescriptorHeap;

class PostEffectBuffer
{
	ShaderBuffer shaderBuffer;
	ColorBuffer colorBuffer;
	
	uint16_t srvHandle;

public:
	
	int atoduke{};
	DepthBuffer depthBuffer;
	int effectType{};

	PostEffectBuffer() {};
	ShaderBuffer* GetShaderBuffer();
	D3D12_CPU_DESCRIPTOR_HANDLE* GetRtvHandle();
	uint16_t* GetSrvHandle();
	void CreateRTV(ID3D12Device* device_, RtvDescriptorHeap* rtvDescHeap_,
		D3D12_RENDER_TARGET_VIEW_DESC desc_);
	inline ColorBuffer* GetColorBuffer() { return &colorBuffer; }
	void CreateDepthBuffer(ID3D12Device* device_, DsvDescriptorHeap* dsvDescriptorHeap_);
};

