#include "SrvCreatorBehavior.h"
#include "../../Data/ShaderBufferData/ShaderBufferData.h"
#include "../../Resource/shaderBuffer/shaderBuffer.h"
#include "../../../../DescriptorHeap/srvDescriptorHeap/srvDescriptorHeap.h"
#pragma comment(lib,"d3d12.lib")

void SrvCreatorBehavior::Init(SrvDescriptorHeap* srvDescriptorHeap_, ID3D12Device* device_,
	CommandControl* commandControl_, ShaderBufferData* shaderBufferData_)
{
	shaderBufferData = shaderBufferData_;
	device = device_;
	commandControl = commandControl_;
	srvDescriptorHeap = srvDescriptorHeap_;
}

uint16_t SrvCreatorBehavior::CreateSRV(D3D12_SHADER_RESOURCE_VIEW_DESC desc_, ShaderBuffer* shaderBuffer_)
{
	//ハンドル
	if (ShaderBuffer::cur_index > 0)
	{
		shaderBuffer_->handle= srvDescriptorHeap->GetCPUDescriptorHandle2(ShaderBuffer::cur_index);
		shaderBuffer_->handleGPU = srvDescriptorHeap->GetGPUDescriptorHandle2(ShaderBuffer::cur_index);

		device->CreateShaderResourceView(shaderBuffer_->resource.Get(), &desc_, shaderBuffer_->handle);
	}
	else
	{
		shaderBuffer_->handle = srvDescriptorHeap->Getter_Descriptorheap()->GetCPUDescriptorHandleForHeapStart();
		shaderBuffer_->handleGPU = srvDescriptorHeap->Getter_Descriptorheap()->GetGPUDescriptorHandleForHeapStart();

		device->CreateShaderResourceView(shaderBuffer_->resource.Get(), &desc_, shaderBuffer_->handle);
	}

	ShaderBuffer::cur_index++;

	if (ShaderBuffer::cur_index > srvDescriptorHeap->Getter_MaxDescriptor()) assert(false);

	return ShaderBuffer::cur_index - 1;
}

SrvCreatorBehavior::SrvCreatorBehavior()
{

}
