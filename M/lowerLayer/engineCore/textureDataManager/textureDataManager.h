#pragma once
#include <string>
#include <vector>
#include "../Buffer/gpuResources/Resource/shaderBuffer/shaderBuffer.h"
#include "../Buffer/PostEffectBuffer/PostEffectBuffer.h"

class TextureSrCreator;
class PostEffectSrCreator;
struct RtvDescriptorHeap;

class TextureDataManager
{
	RtvDescriptorHeap* rtvDescriptorHeap = nullptr;
	TextureSrCreator* textureSrvCreator = nullptr;
	PostEffectSrCreator* postEffectSrCreator = nullptr;
	ID3D12Device* device = nullptr;
	std::vector<ShaderBuffer> data;
	std::vector<std::unique_ptr<PostEffectBuffer>> postEffectBufferContaier;

public:

	void Init(TextureSrCreator* textureSrvCreator_, PostEffectSrCreator* postEffectSrCreator_,
		RtvDescriptorHeap* rtvDescriptorHeap_, ID3D12Device* device_);
	int CreateTextureFromFile(std::string filePath_);
	PostEffectBuffer* CreatePostEffectBuffer(float width_ = 1280.0f, float height_ = 720.0f);
};

