#pragma once

#include "../../../gpuResourcesBehavior.h"

struct RtvDescriptorHeap;

struct ColorBuffer:GPUResourceBehavior
{
	float width = 1280.0f;
	float height = 720.0f;

private:

	//[ currentDescriptorUsingIndex ]
	static inline uint16_t cur_index = 0;

	D3D12_VIEWPORT viewport;
	D3D12_RECT scissorRect;


public:
	inline auto* Getter_ViewportMatrix()
	{
		return &viewport;
	}

	inline auto* ScissorRect()
	{
		return &scissorRect;
	}


	void SetDXMatrix(float width_, float height_, float left_ = 0.0f, float top_ = 0.0f);

	[[nodiscard]] static D3D12_RENDER_TARGET_VIEW_DESC CreateRTV_Desc(DXGI_FORMAT format_ = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		D3D12_RTV_DIMENSION dimension_ = D3D12_RTV_DIMENSION_TEXTURE2D,
		UINT planeSlice_ = 0, UINT mipSlice_ = 0);

	void CreateRTV(ID3D12Device* device_, RtvDescriptorHeap* rtvDescHeap_, 
		D3D12_RENDER_TARGET_VIEW_DESC desc_, ID3D12Resource* resource_);

};

