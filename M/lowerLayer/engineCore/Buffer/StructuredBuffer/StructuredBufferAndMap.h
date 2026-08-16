#pragma once
#include "../gpuResources/Resource/shaderBuffer/shaderBuffer.h"
#include "../gpuResourcesBehavior.h"
#include <assert.h>
#include <memory>

template <typename T>
struct StructuredBufferAndMap
{
public:
	uint16_t gpuHandleIndex{};
	ShaderBuffer shaderBuffer;
	T* buffMap = nullptr;

	void CreateAndMapping(ID3D12Device* device_,UINT maxCreateNum_)
	{
		//リソースのサイズ
		UINT sizeOfBuffer = static_cast<UINT>(sizeof(T)* maxCreateNum_);
		//Resourceの生成
		shaderBuffer.resource = CreateBufferResource(device_, sizeOfBuffer);
		//書き込むためのアドレスを取得
		HRESULT hr = shaderBuffer.resource->Map(0, nullptr, reinterpret_cast<void**>(&buffMap));
		assert(SUCCEEDED(hr));
	}

};



