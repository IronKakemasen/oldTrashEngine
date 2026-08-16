#pragma once
#include "../../../gpuResourcesBehavior.h"

struct DsvDescriptorHeap;

struct DepthBuffer:GPUResourceBehavior
{
private:
	//[ currentDescriptorUsingIndex ]
	static inline uint16_t cur_index = 0;

public:
	static inline int atoduke{};

	[[nodiscard]] D3D12_DEPTH_STENCIL_VIEW_DESC CreateDepthStencilViewDesc(
		DXGI_FORMAT format_ = DXGI_FORMAT_D32_FLOAT_S8X24_UINT,
		D3D12_DSV_DIMENSION dimension_ = D3D12_DSV_DIMENSION_TEXTURE2D,
		UINT mipSlice_ = 0,
		D3D12_DSV_FLAGS flags_ = D3D12_DSV_FLAG_NONE
	);
	void CreateDSV(ID3D12Device* device_, DsvDescriptorHeap* dsvDescHeap_, D3D12_DEPTH_STENCIL_VIEW_DESC desc_);
	void CreateDepthStencilTextureResource(ID3D12Device* device_ , int32_t width_, int32_t height_);

};

