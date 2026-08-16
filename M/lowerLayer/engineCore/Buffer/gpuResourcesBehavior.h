#pragma once

#include <wrl.h>
#include <d3d12.h>
#include<stdint.h>

class GPUResourceBehavior
{
public:

	Microsoft::WRL::ComPtr<ID3D12Resource> resource = { nullptr };
	D3D12_CPU_DESCRIPTOR_HANDLE handle;

	inline void Setter_Resource(Microsoft::WRL::ComPtr<ID3D12Resource> src_)
	{
		resource = src_;
	}

	inline auto** Getter_ResourcePP()
	{
		return resource.GetAddressOf();
	}

	inline auto* Getter_Resource()
	{
		return resource.Get();
	}

	inline auto* Getter_Handle()
	{
		return &handle;
	}
};

[[nodiscard]] Microsoft::WRL::ComPtr<ID3D12Resource> CreateBufferResource(
	ID3D12Device* device_, UINT sizeInByte_);


