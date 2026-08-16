#include "PostEffects.h"
#include "../engineCore/textureDataManager/textureDataManager.h"
#include "../../M.h"
#include "../../../commonVariables.h"
#include "../OffScreenManager.h"

PostEffects::NoEffection::NoEffection(TextureDataManager* textureDataManager_, int numRequired_, OriginalScreen* originalScreen_, float width_, float height_) :
	OffScreen(textureDataManager_, numRequired_, originalScreen_, width_, height_) {
}

void PostEffects::NoEffection::Update()
{

}

void PostEffects::NoEffection::Init(ID3D12Device* device_)
{
	shaderSetIndex = M::GetInstance()->
		GetShaderSetIndexFromFileName("NoEffection.VS", "NoEffection.PS");
	useTextures.emplace_back(*originalScreen->buffers[0]->GetSrvHandle());

}

PostEffects::GreyScale::GreyScale(TextureDataManager* textureDataManager_, int numRequired_, OriginalScreen* originalScreen_, float width_, float height_) :
	OffScreen(textureDataManager_, numRequired_, originalScreen_, width_, height_) {
}

void PostEffects::GreyScale::Update()
{

}

void PostEffects::GreyScale::Init(ID3D12Device* device_)
{
	shaderSetIndex = M::GetInstance()->
		GetShaderSetIndexFromFileName("MonoChrome.VS", "MonoChrome.PS");

}

void PostEffects::SimpleNeonLike::Update()
{
	cBuffer.buffer.buffMap->backgroundDim = para.backgroundDim;
	cBuffer.buffer.buffMap->edgeWidth = para.edgeWidth;
	cBuffer.buffer.buffMap->texelSize = para.texelSize;

}

void PostEffects::SimpleNeonLike::Init(ID3D12Device* device_)
{
	shaderSetIndex = M::GetInstance()->
		GetShaderSetIndexFromFileName("SimpleNeonLike.VS", "SimpleNeonLike.PS");

	cBuffer.buffer.CreateAndMapping(device_);
	cBufferAddressContainer.emplace_back(cBuffer.buffer.GetVirtualGPUAddress());

	cBuffer.buffer.buffMap->backgroundDim = para.backgroundDim;
	cBuffer.buffer.buffMap->edgeWidth = para.edgeWidth;
	cBuffer.buffer.buffMap->texelSize = para.texelSize;

}

PostEffects::SimpleNeonLike::SimpleNeonLike(TextureDataManager* textureDataManager_, int numRequired_, OriginalScreen* originalScreen_, float width_, float height_) :
	OffScreen(textureDataManager_, numRequired_, originalScreen_, width_, height_)
{
	para.backgroundDim = 0.01f;
	para.edgeWidth = 1.0f;
	//para.texelSize = { 1.0f / CommonV::kWindow_W,1.0f / CommonV::kWindow_H };
	para.texelSize = { 0.002f,0.002f };

}


PostEffects::SideBlur::SideBlur(TextureDataManager* textureDataManager_, int numRequired_, OriginalScreen* originalScreen_, float width_, float height_, Brightness* brightness_) :
	OffScreen(textureDataManager_, numRequired_, originalScreen_, width_, height_)
{
	shaderSetIndex = M::GetInstance()->
		GetShaderSetIndexFromFileName("SideBlur.VS", "SideBlur.PS");
	blendMode = kBlendModeAdd;
	brightness = brightness_;

}

void PostEffects::SideBlur::Update()
{
	offScreenManager->DrawOnResource(buffers[0], PostEffectType::kSideBlur);

}

void PostEffects::SideBlur::Init(ID3D12Device* device_)
{
	gaussianBlurparaBuffer.buffMap.CreateAndMapping(device_);
	gaussianBlurparaBuffer.buffMap.buffMap->Init();

	cBufferAddressContainer.emplace_back(gaussianBlurparaBuffer.buffMap.GetVirtualGPUAddress());
	useTextures.emplace_back(*originalScreen->buffers[0]->GetSrvHandle());
	useTextures.emplace_back(*originalScreen->buffers[0]->GetSrvHandle());

}


PostEffects::VerticalBlur::VerticalBlur(TextureDataManager* textureDataManager_, int numRequired_, OriginalScreen* originalScreen_, float width_, float height_) :
	OffScreen(textureDataManager_, numRequired_, originalScreen_, width_, height_)
{
	shaderSetIndex = M::GetInstance()->
		GetShaderSetIndexFromFileName("VerticalBlur.VS", "VerticalBlur.PS");
	blendMode = kBlendModeAdd;

}

void PostEffects::VerticalBlur::Update()
{
	offScreenManager->DrawOnResource(buffers[0], PostEffectType::kVerticalBlur);

}

void PostEffects::VerticalBlur::Init(ID3D12Device* device_)
{
	gaussianBlurparaBuffer.buffMap.CreateAndMapping(device_);
	gaussianBlurparaBuffer.buffMap.buffMap->Init();

	cBufferAddressContainer.emplace_back(gaussianBlurparaBuffer.buffMap.GetVirtualGPUAddress());
}


PostEffects::Half::Half(TextureDataManager* textureDataManager_, int numRequired_, OriginalScreen* originalScreen_, float width_, float height_,VerticalBlur* verticalBlur_) :
	OffScreen(textureDataManager_, numRequired_, originalScreen_, width_, height_)
{
	verticalBlur = verticalBlur_;
	blendMode = kBlendModeAdd;

}

void PostEffects::Half::Update()
{
	offScreenManager->DrawOnResource(buffers[0], PostEffectType::kHalf);
}

void PostEffects::Half::Init(ID3D12Device* device_)
{
	shaderSetIndex = M::GetInstance()->
		GetShaderSetIndexFromFileName("SideBlur.VS", "SideBlur.PS");
	useTextures.emplace_back(*verticalBlur->buffers[0]->GetSrvHandle());
	useTextures.emplace_back(*verticalBlur->buffers[0]->GetSrvHandle());

	gaussianBlurparaBuffer.buffMap.CreateAndMapping(device_);
	gaussianBlurparaBuffer.buffMap.buffMap->Init();

	cBufferAddressContainer.emplace_back(gaussianBlurparaBuffer.buffMap.GetVirtualGPUAddress());

}

PostEffects::Quater::Quater(TextureDataManager* textureDataManager_, int numRequired_, OriginalScreen* originalScreen_, float width_, float height_,Half* half_) :
	OffScreen(textureDataManager_, numRequired_, originalScreen_, width_, height_) 
{
	half = half_;
	blendMode = kBlendModeAdd;

}

void PostEffects::Quater::Update()
{
	offScreenManager->DrawOnResource(buffers[0], PostEffectType::kQuater);
}

void PostEffects::Quater::Init(ID3D12Device* device_)
{
	shaderSetIndex = M::GetInstance()->
		GetShaderSetIndexFromFileName("VerticalBlur.VS", "VerticalBlur.PS");

	useTextures.emplace_back(*half->buffers[0]->GetSrvHandle());
	useTextures.emplace_back(*half->buffers[0]->GetSrvHandle());

	gaussianBlurparaBuffer.buffMap.CreateAndMapping(device_);
	gaussianBlurparaBuffer.buffMap.buffMap->Init();

	cBufferAddressContainer.emplace_back(gaussianBlurparaBuffer.buffMap.GetVirtualGPUAddress());

}

PostEffects::GaussianBlur::GaussianBlur(TextureDataManager* textureDataManager_, int numRequired_, OriginalScreen* originalScreen_, float width_, float height_,
	SideBlur* sideBlur_, VerticalBlur* verticalBlur_, Brightness* brightness_) :
	OffScreen(textureDataManager_, numRequired_, originalScreen_, width_, height_)
{
	shaderSetIndex = M::GetInstance()->
		GetShaderSetIndexFromFileName("GaussianBlur.VS", "GaussianBlur.PS");
	
	sideBlur = sideBlur_;
	verticalBlur = verticalBlur_;
	brightness = brightness_;

}

void PostEffects::GaussianBlur::Update()
{
	offScreenManager->GetPalette()->paraContainer[ PostEffectType::kSideBlur].
		offscreenTextureContainer[0] = *originalScreen->buffers[0]->GetSrvHandle();

	offScreenManager->GetPalette()->paraContainer[PostEffectType::kSideBlur].
		offscreenTextureContainer[1] = *originalScreen->buffers[0]->GetSrvHandle();

	sideBlur->Update();
	verticalBlur->Update();

	offScreenManager->DrawOnResource(buffers[0], PostEffectType::kGaussianBlur);

}

void PostEffects::GaussianBlur::Init(ID3D12Device* device_)
{
	useTextures.emplace_back(*sideBlur->GetBuffer(0)->GetSrvHandle());
	useTextures.emplace_back(*verticalBlur->GetBuffer(0)->GetSrvHandle());
}

PostEffects::Brightness::Brightness(TextureDataManager* textureDataManager_, int numRequired_, OriginalScreen* originalScreen_, float width_, float height_) :
	OffScreen(textureDataManager_, numRequired_, originalScreen_, width_, height_)
{
	originalScreen = originalScreen_;

	shaderSetIndex = M::GetInstance()->
		GetShaderSetIndexFromFileName("Brightness.VS", "Brightness.PS");
	useTextures.emplace_back(*originalScreen->buffers[0]->GetSrvHandle());

	blendMode = kBlendModeAdd;
}

void PostEffects::Brightness::Update()
{
	offScreenManager->DrawOnResource(buffers[0], PostEffectType::kBrightness);
}

void PostEffects::Brightness::Init(ID3D12Device* device_)
{

}

PostEffects::Bloom::Bloom(TextureDataManager* textureDataManager_, int numRequired_, OriginalScreen* originalScreen_, float width_, float height_,
	Brightness* brightness_, SideBlur* sideBlur_, VerticalBlur* verticalBlur_, Half* half_, Quater* quater_) :
	OffScreen(textureDataManager_, numRequired_, originalScreen_, width_, height_)
{
	shaderSetIndex = M::GetInstance()->
		GetShaderSetIndexFromFileName("Bloom.VS", "Bloom.PS");

	brightness = brightness_;
	sideBlur = sideBlur_; 
	verticalBlur = verticalBlur_;
	half = half_; 
	quater = quater_;

	verticalBlur->useTextures.emplace_back(*sideBlur->buffers[0]->GetSrvHandle());
	verticalBlur->useTextures.emplace_back(*sideBlur->buffers[0]->GetSrvHandle());

	useTextures.emplace_back(*sideBlur->buffers[0]->GetSrvHandle());
	useTextures.emplace_back(*verticalBlur->buffers[0]->GetSrvHandle());
	useTextures.emplace_back(*half->buffers[0]->GetSrvHandle());
	useTextures.emplace_back(*quater->buffers[0]->GetSrvHandle());
	useTextures.emplace_back(*originalScreen->buffers[0]->GetSrvHandle());

	blendMode = kBlendModeNormal;
}

void PostEffects::Bloom::Update()
{
	brightness->Update();

	offScreenManager->GetPalette()->paraContainer[PostEffectType::kSideBlur].
		offscreenTextureContainer[0] = *brightness->buffers[0]->GetSrvHandle();

	offScreenManager->GetPalette()->paraContainer[PostEffectType::kSideBlur].
		offscreenTextureContainer[1] = *brightness->buffers[0]->GetSrvHandle();

	sideBlur->Update();
	verticalBlur->Update();
	half->Update();
	quater->Update();

}

void PostEffects::Bloom::Init(ID3D12Device* device_)
{

}


PostEffects::DepthOfField::DepthOfField(TextureDataManager* textureDataManager_, int numRequired_, OriginalScreen* originalScreen_, float width_, float height_, DOF_Screen* dOF_Screen_, GaussianBlur* gaussianBlur_) :
	OffScreen(textureDataManager_, numRequired_, originalScreen_, width_, height_) 
{
	gaussianBlur = gaussianBlur_;
	dOF_Screen = dOF_Screen_;
}

void PostEffects::DepthOfField::Update()
{
	gaussianBlur->Update();

	//offScreenManager->DrawOnResource(buffers[0], PostEffectType::kDepthOfField);

}

void PostEffects::DepthOfField::Init(ID3D12Device* device_)
{
	shaderSetIndex = M::GetInstance()->
		GetShaderSetIndexFromFileName("DepthOfView.VS", "DepthOfView.PS");
	useTextures.emplace_back(*originalScreen->buffers[0]->GetSrvHandle());
	useTextures.emplace_back(*dOF_Screen->buffers[0]->GetSrvHandle());
	useTextures.emplace_back(*gaussianBlur->buffers[0]->GetSrvHandle());

}


