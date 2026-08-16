#pragma once
#include "../../Resource/shaderBuffer/shaderBuffer.h"
#include < vector>

struct ShaderBufferData
{
	std::vector<D3D12_GPU_DESCRIPTOR_HANDLE> gpuHandleContainer;

};

