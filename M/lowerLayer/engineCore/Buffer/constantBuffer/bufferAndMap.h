#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <assert.h>
#include "../gpuResourcesBehavior.h"

template <typename T>
struct BufferAndMap
{
private:
	Microsoft::WRL::ComPtr<ID3D12Resource> buff = nullptr;

public:
	T* buffMap = nullptr;

	D3D12_GPU_VIRTUAL_ADDRESS GetVirtualGPUAddress()
	{
		return buff->GetGPUVirtualAddress();
	}

	void CreateAndMapping(ID3D12Device* device_)
	{
		//リソースのサイズ
		UINT sizeOfBuffer = static_cast<UINT>(sizeof(T));
		//Resourceの生成
		buff = CreateBufferResource(device_, sizeOfBuffer);
		//書き込むためのアドレスを取得
		HRESULT hr = buff->Map(0, nullptr, reinterpret_cast<void**>(&buffMap));
		SUCCEEDED(hr);
	}

	inline auto* Geter_Resource()
	{
		return buff.Get();
	}

};



