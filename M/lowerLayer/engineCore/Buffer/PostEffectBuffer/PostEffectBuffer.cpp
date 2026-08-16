#include "PostEffectBuffer.h"
#include "../../../commonVariables.h"
#include "../Buffer/gpuResources/Creator/SrvCreator/PostEffectSrCreator/PostEffectSrCreator.h"


void PostEffectBuffer::CreateDepthBuffer(ID3D12Device* device_, DsvDescriptorHeap* dsvDescriptorHeap_)
{
	depthBuffer.CreateDepthStencilTextureResource(device_, (int)colorBuffer.width, (int)colorBuffer.height);
	auto desc = depthBuffer.CreateDepthStencilViewDesc();
	depthBuffer.CreateDSV(device_, dsvDescriptorHeap_, desc);
}


void PostEffectBuffer::CreateRTV(ID3D12Device* device_, RtvDescriptorHeap* rtvDescHeap_,
	D3D12_RENDER_TARGET_VIEW_DESC desc_)
{
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = ColorBuffer::CreateRTV_Desc(PostEffectSrCreator::commonFormat);

	colorBuffer.CreateRTV(device_, rtvDescHeap_, rtvDesc, shaderBuffer.Getter_Resource());
	colorBuffer.SetDXMatrix(colorBuffer.width, colorBuffer.height);

}

ShaderBuffer* PostEffectBuffer::GetShaderBuffer()
{
	return &shaderBuffer;
}

D3D12_CPU_DESCRIPTOR_HANDLE* PostEffectBuffer::GetRtvHandle()
{
	return colorBuffer.Getter_Handle();
}

uint16_t* PostEffectBuffer::GetSrvHandle()
{
	return &srvHandle;
}