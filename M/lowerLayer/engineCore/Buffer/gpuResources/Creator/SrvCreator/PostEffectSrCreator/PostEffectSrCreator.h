#pragma once
#include "../SrvCreatorBehavior.h"
#include <wrl.h>
#include <d3d12.h>
#include <memory>

class PostEffectBuffer;
struct DsvDescriptorHeap;

class PostEffectSrCreator :public SrvCreatorBehavior
{
	D3D12_SHADER_RESOURCE_VIEW_DESC CreateSrvDesc();
	void CreateSRVForPostEffect(PostEffectBuffer* data_);
	void CreteResource(PostEffectBuffer* data_);

public:

	//static inline DXGI_FORMAT commonFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;

	static inline DXGI_FORMAT commonFormat = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	static inline DsvDescriptorHeap* dsvDescriptorHeap;
	std::unique_ptr<PostEffectBuffer> Create(float width_ = 1280.0f, float height_ = 720.0f);
	PostEffectSrCreator();
};
