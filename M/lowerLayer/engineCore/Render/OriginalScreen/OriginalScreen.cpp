#include "OriginalScreen.h"
#include "../engineCore/textureDataManager/textureDataManager.h"
#include "../M.h"

void OffScreenBehavior::Set(OffScreenManager* offScreenManager_, int effectType_) 
{
	offScreenManager = offScreenManager_;
	if (buffers.size()>0)
	{
		buffers[0]->effectType = effectType_;
	}
}

OffScreenBehavior::OffScreenBehavior(TextureDataManager* textureDataManager_, int numRequired_, OriginalScreen* originalScreen_
	, float width_, float height_)
{
	for (int i = 0; i < numRequired_; ++i)
	{
		buffers.emplace_back(textureDataManager_->CreatePostEffectBuffer());
	}

	originalScreen = originalScreen_;
}

PostEffectBuffer* OffScreenBehavior::GetBuffer(int index_)
{
	return buffers[index_];
}

std::vector<uint16_t> OffScreenBehavior::WatchTextureIndexes()
{
	std::vector<uint16_t> ret;
	for (auto* it : buffers)
	{
		if (it)
		{
			ret.emplace_back(*it->GetSrvHandle());
		}
	}
	return ret;
}

int OffScreenBehavior::WathchShaderSetIndex()
{
	return shaderSetIndex;
}

std::vector<D3D12_GPU_VIRTUAL_ADDRESS> OffScreenBehavior::WatchAddessContainer()
{
	return cBufferAddressContainer;
}



OffScreen::OffScreen(TextureDataManager* textureDataManager_, int numRequired_, OriginalScreen* originalScreen_ ,float width_, float height_) :
	OffScreenBehavior(textureDataManager_, numRequired_, originalScreen_, width_,height_) {
};


OriginalScreen::OriginalScreen(TextureDataManager* textureDataManager_, int numRequired_, OriginalScreen* originalScreen_, float width_, float height_) :
	OffScreen(textureDataManager_, numRequired_, originalScreen_, width_, height_) {
};

void OriginalScreen::Init(ID3D12Device* device_)
{
	shaderSetIndex = M::GetInstance()->
		GetShaderSetIndexFromFileName("NoEffection.VS", "NoEffection.PS");

}

void OriginalScreen::Update()
{

}
