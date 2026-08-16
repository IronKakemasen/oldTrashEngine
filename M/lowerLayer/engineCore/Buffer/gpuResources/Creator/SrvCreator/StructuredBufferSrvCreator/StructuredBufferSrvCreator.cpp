#include "StructuredBufferSrvCreator.h"
#include "../../../Data/ShaderBufferData/ShaderBufferData.h"

D3D12_SHADER_RESOURCE_VIEW_DESC StructuredBufferSrvCreator::CreateSRVDesc(UINT createNum_, UINT size_)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC ret_desc{};

	ret_desc.Format = DXGI_FORMAT_UNKNOWN;
	ret_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	ret_desc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;

	ret_desc.Buffer.FirstElement = 0;
	ret_desc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	ret_desc.Buffer.NumElements = createNum_;
	ret_desc.Buffer.StructureByteStride = size_;

	return ret_desc;

}

uint16_t StructuredBufferSrvCreator::CreateSRVForStructuredBuffer(UINT createNum_, UINT size_,ShaderBuffer& data_)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC desc = CreateSRVDesc(createNum_, size_);
	uint16_t ret_index = CreateSRV(desc, &data_);

	shaderBufferData->gpuHandleContainer.emplace_back(data_.handleGPU);

#ifdef USE_IMGUI
	return ret_index - 1;
#endif // USE_IMGUI

	return ret_index;

}
