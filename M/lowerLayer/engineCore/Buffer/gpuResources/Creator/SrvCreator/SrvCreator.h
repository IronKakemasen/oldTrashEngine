#pragma once
#include "./TextureSrCreator/TextureSrCreator.h"
#include "./ParticleMeshSrvCreator/ParticleMeshSrvCreator.h"
#include "./StructuredBufferSrvCreator/StructuredBufferSrvCreator.h"
#include "./PostEffectSrCreator/PostEffectSrCreator.h"

struct ShaderBuffer;
struct SrvDescriptorHeap;
class CommandControl;
struct ShaderBufferData;
struct ID3D12Device;

class SrvCreator
{
	TextureSrCreator textureSrvCreator;
	ParticleMeshSrvCreator particleMeshSrvCreator;
	StructuredBufferSrvCreator stBufferCretaor;
	PostEffectSrCreator postEffectSrvCreator;

public:
	void Init(SrvDescriptorHeap* srvDescriptorHeap_, ID3D12Device* device_,
		CommandControl* commandControl_, ShaderBufferData* shaderBufferData_);

	auto* Getter_ParticleMeshSrvCreator()
	{
		return &particleMeshSrvCreator;
	}

	auto* Getter_TextureSrCreator()
	{
		return &textureSrvCreator;
	}

	auto* Getter_StBufferCretaor()
	{
		return &stBufferCretaor;
	}

	auto* Getter_PostEffectSrCreator()
	{
		return &postEffectSrvCreator;
	}


};

