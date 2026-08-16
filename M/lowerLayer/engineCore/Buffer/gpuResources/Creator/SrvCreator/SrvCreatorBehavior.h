#pragma once
#include <d3d12.h>
#include <stdint.h>

struct ShaderBuffer;
struct SrvDescriptorHeap;
class CommandControl;
struct ShaderBufferData;

struct SrvCreatorBehavior
{
protected:
	ShaderBufferData* shaderBufferData = nullptr;
	ID3D12Device* device = nullptr;
	CommandControl* commandControl = nullptr;
	SrvDescriptorHeap* srvDescriptorHeap = nullptr;

	uint16_t CreateSRV(D3D12_SHADER_RESOURCE_VIEW_DESC desc_, ShaderBuffer* shaderBuffer_);

public:

	void Init(SrvDescriptorHeap* srvDescriptorHeap_, ID3D12Device* device_,
		CommandControl* commandControl_, ShaderBufferData* shaderBufferData_);

	SrvCreatorBehavior();

	inline auto* Getter_ShaderBufferData()
	{
		return &shaderBufferData;
	}

};
