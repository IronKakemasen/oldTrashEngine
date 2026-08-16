#include "SrvCreator.h"


void SrvCreator::Init(SrvDescriptorHeap* srvDescriptorHeap_, ID3D12Device* device_,
	CommandControl* commandControl_, ShaderBufferData* shaderBufferData_)
{
	textureSrvCreator.Init(srvDescriptorHeap_, device_,
		commandControl_, shaderBufferData_);
	particleMeshSrvCreator.Init(srvDescriptorHeap_, device_,
		commandControl_, shaderBufferData_);
	stBufferCretaor.Init(srvDescriptorHeap_, device_,
		commandControl_, shaderBufferData_);
	postEffectSrvCreator.Init(srvDescriptorHeap_, device_,
		commandControl_, shaderBufferData_);

}
