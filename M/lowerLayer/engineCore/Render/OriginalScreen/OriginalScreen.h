#pragma once
#include <vector>
#include <d3d12.h>
#include "../../PSO/pipelineCreators/pipelineComponents.h"

class PostEffectBuffer;
class TextureDataManager;
class OriginalScreen;
class OffScreenManager;

class OffScreenBehavior
{
protected:
	std::vector<D3D12_GPU_VIRTUAL_ADDRESS> cBufferAddressContainer;
	OriginalScreen* originalScreen = nullptr;
	OffScreenManager* offScreenManager;

public:
	BlendMode blendMode = kBlendModeNormal;
	std::vector<PostEffectBuffer*> buffers;
	std::vector<uint16_t> useTextures;
	int shaderSetIndex{};

	void Set(OffScreenManager* offScreenManager_, int effectType_);
	OffScreenBehavior(TextureDataManager* textureDataManager_, int numRequired_, 
		OriginalScreen* originalScreen_,float width_ = 1280.0f, float height_ = 720.0f);
	virtual void Init(ID3D12Device* device_) = 0;
	virtual void Update() = 0;
	PostEffectBuffer* GetBuffer(int index_);
	std::vector<uint16_t> WatchTextureIndexes();
	int WathchShaderSetIndex();
	std::vector<D3D12_GPU_VIRTUAL_ADDRESS> WatchAddessContainer();
};

class OffScreen:public OffScreenBehavior
{

public:
	virtual void Init(ID3D12Device* device_)override {};
	virtual void Update()override {};
	OffScreen(TextureDataManager* textureDataManager_, int numRequired_, OriginalScreen* originalScreen_ , float width_ , float height_ );
};


class OriginalScreen:public OffScreen
{

public:
	virtual void Init(ID3D12Device* device_) override;
	virtual void Update() override;
	OriginalScreen(TextureDataManager* textureDataManager_, int numRequired_, OriginalScreen* originalScreen_ , float width_, float height_);

};

