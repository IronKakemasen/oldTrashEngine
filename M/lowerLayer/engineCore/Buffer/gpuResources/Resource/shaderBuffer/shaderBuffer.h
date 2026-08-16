#pragma once
#include "../../../gpuResourcesBehavior.h"
#include "../../../../../../../external/DirectXTex/d3dx12.h"
#include "../../../../../../../external/DirectXTex/DirectXTex.h"

struct SrvDescriptorHeap;
class CommandControl;

struct ShaderBuffer :GPUResourceBehavior
{
	//[ currentDescriptorUsingIndex ]
	static inline uint16_t cur_index = 0;

	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU;
	Microsoft::WRL::ComPtr<ID3D12Resource> intermediateResource;

public:
	ShaderBuffer() {};
};

