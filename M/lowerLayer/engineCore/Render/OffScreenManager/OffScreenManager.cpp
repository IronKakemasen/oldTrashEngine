#include "OffScreenManager.h"
#include "../engineCore/textureDataManager/textureDataManager.h"
#include "../engineCore/Essential/BarrierControl/BarrierControl.h"
#include "../engineCore/Essential/CommandControl/CommandControl.h"
#include "../engineCore/Essential/SwapChainControl/SwapChainControl.h"
#include "../engineCore/DescriptorHeap/srvDescriptorHeap/srvDescriptorHeap.h"
#include "../../engineCore/WinApp.h"
#include "../Palette/Palette.h"
#include "../../M.h"


DOF_Screen::DOF_Screen(TextureDataManager* textureDataManager_, int numRequired_, OriginalScreen* originalScreen_, float width_, float height_) :
	OffScreen(textureDataManager_, numRequired_, originalScreen_, width_, height_)
{

};

void DOF_Screen::Init(ID3D12Device* device_)
{
	shaderSetIndex = M::GetInstance()->
		GetShaderSetIndexFromFileName("NoEffection.VS", "NoEffection.PS");

}

void DOF_Screen::Update()
{

}


void OffScreenManager::PostEffectBegin(PostEffectBuffer* srcBuffer_, PostEffectType postEffectType_)
{
	auto* cList = commandControl->Getter_commandList();
	auto* depthHandle = srcBuffer_->depthBuffer.Getter_Handle();
	auto* originalScreen = GetOriginalScreen();
	auto* rtvHandle = srcBuffer_->GetRtvHandle();

	auto pre_effectType = ChangePostEffection(postEffectType_);

	int buff = srcBuffer_->atoduke;
	preRenderTargetStat = curRenderTargetStatus;
	curRenderTargetStatus = buff;

	D3D12_RESOURCE_BARRIER barrier = BarrierControl::Create(
		srcBuffer_->GetShaderBuffer()->Getter_Resource(),
		D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
		D3D12_RESOURCE_BARRIER_FLAG_NONE,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES);

	BarrierControl::Pitch(commandControl->Getter_commandList(), &barrier);

	cList->OMSetRenderTargets(1, rtvHandle, false, depthHandle);

	//指定した深度で画面クリアする
	//cList->ClearDepthStencilView(*depthHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	cList->ClearDepthStencilView(*depthHandle, D3D12_CLEAR_FLAG_DEPTH, 0.0f, 0, 0, nullptr);

	//指定した色で画面全体をクリアする
	float palleteColor[4] = { 0.0f,0.0f,0.0f,0.0f };

	//指定した色で画面全体をクリアする
	commandControl->Getter_commandList()->ClearRenderTargetView(
		*rtvHandle, palleteColor, 0, nullptr);

	////描画用のDescriptorHeapの設定
	ID3D12DescriptorHeap* descriptorHeaps[] = { srvDescriptorHeap->Getter_Descriptorheap() };
	commandControl->Getter_commandList()->SetDescriptorHeaps(1, descriptorHeaps);

	//行列
	commandControl->Getter_commandList()->RSSetViewports(1,
		srcBuffer_->GetColorBuffer()->Getter_ViewportMatrix());

	commandControl->Getter_commandList()->RSSetScissorRects(1,
		srcBuffer_->GetColorBuffer()->ScissorRect());

	exclusiveDraw->DrawOnPalette(&palette );
	//drawIndexをリセットする
	exclusiveDraw->ResetDrawIndexes();

	ChangePostEffection(pre_effectType);

	if (palette.curType == PostEffectType::kBloom)
	{
		exclusiveDraw->dirLightBuffer->dirLight.buffMap->intensity = 10.0f;
	}
	else
	{
		exclusiveDraw->dirLightBuffer->dirLight.buffMap->intensity = 1.0f;
	}

}

void OffScreenManager::DrawOnResource(PostEffectBuffer* srcBuffer_, PostEffectType postEffectType_)
{
	PostEffectBegin(srcBuffer_, postEffectType_);
	PostEffectEnd(srcBuffer_);
}

void OffScreenManager::PostEffectEnd(PostEffectBuffer* srcBuffer_)
{
	D3D12_RESOURCE_BARRIER barrier = BarrierControl::Create(
		srcBuffer_->GetShaderBuffer()->Getter_Resource(),
		D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
		D3D12_RESOURCE_BARRIER_FLAG_NONE,
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES);

	BarrierControl::Pitch(commandControl->Getter_commandList(), &barrier);

	curRenderTargetStatus = preRenderTargetStat;
}

void OffScreenManager::Init(TextureDataManager* textureDataManager_,
	ExclusiveDraw* exclusiveDraw_, ID3D12Device* device_,
	CommandControl* commandControl_, SwapChainControl* swapChainControl_, SrvDescriptorHeap* srvDescriptorHeap_)
{
	srvDescriptorHeap = srvDescriptorHeap_;
	commandControl = commandControl_;
	swapChainControl = swapChainControl_;
	textureDataManager = textureDataManager_;
	exclusiveDraw = exclusiveDraw_;

	originalScreen.reset(new OriginalScreen(textureDataManager_, 1,nullptr,1280.0f, 720.0f));
	originalScreen->Init(device_);


	palette.Init(originalScreen.get());

	SetEffectSysytem(PostEffectType::kNone,
		std::make_unique<PostEffects::NoEffection>(textureDataManager_, 0,originalScreen.get(),1280.0f , 720.0f));
	SetEffectSysytem(PostEffectType::kGreyScale,
		std::make_unique<PostEffects::GreyScale>(textureDataManager_, 0, originalScreen.get(), 1280.0f, 720.0f));
	SetEffectSysytem(PostEffectType::kSimpleNeonLike,
		std::make_unique<PostEffects::SimpleNeonLike>(textureDataManager_, 0, originalScreen.get(), 1280.0f, 720.0f));
	
	//PostEffectSrCreator::commonFormat = DXGI_FORMAT_R32_FLOAT;
	PostEffectSrCreator::commonFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;

	dOF_Screen.reset(new DOF_Screen(textureDataManager_, 1, originalScreen.get(), 1280.0f, 720.0f));

	PostEffectSrCreator::commonFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;

	auto* brightness = dynamic_cast<PostEffects::Brightness*>(SetEffectSysytem(PostEffectType::kBrightness,
		std::make_unique<PostEffects::Brightness>(textureDataManager_, 1, originalScreen.get(), 1280.0f, 720.0f)));

	auto* sBlur = dynamic_cast<PostEffects::SideBlur*> (SetEffectSysytem(PostEffectType::kSideBlur,
		std::make_unique<PostEffects::SideBlur>(textureDataManager_, 1, originalScreen.get(), 640.0f, 360.0f, brightness)));

	auto* vBlur = dynamic_cast<PostEffects::VerticalBlur*> (SetEffectSysytem(PostEffectType::kVerticalBlur,
		std::make_unique<PostEffects::VerticalBlur>(textureDataManager_, 1, originalScreen.get(), 320.0f, 180.0f)));
	auto* half = dynamic_cast<PostEffects::Half*> (SetEffectSysytem(PostEffectType::kHalf,
		std::make_unique<PostEffects::Half>(textureDataManager_, 1, originalScreen.get(), 160.0f, 90.0f, vBlur)));
	auto* quater = dynamic_cast<PostEffects::Quater*> (SetEffectSysytem(PostEffectType::kQuater,
		std::make_unique<PostEffects::Quater>(textureDataManager_, 1, originalScreen.get(), 80.0f, 45.0f, half)));
	auto* gBlur = dynamic_cast<PostEffects::GaussianBlur*>(SetEffectSysytem(PostEffectType::kGaussianBlur,
		std::make_unique<PostEffects::GaussianBlur>(textureDataManager_, 1, originalScreen.get(), 1280.0f, 720.0f, sBlur, vBlur, brightness)));
	
	SetEffectSysytem(PostEffectType::kBloom,
		std::make_unique<PostEffects::Bloom>(textureDataManager_, 0, originalScreen.get(), 1280.0f, 720.0f,
			brightness, sBlur, vBlur, half, quater));

	SetEffectSysytem(PostEffectType::kDepthOfField,
		std::make_unique<PostEffects::DepthOfField>(textureDataManager_, 0, originalScreen.get(), 1280.0f, 720.0f, dOF_Screen.get(), gBlur));


	for (int i = 0; i < (int)PostEffectType::kCount; ++i)
	{
		auto* it = effectContainer[PostEffectType(i)].get();

		it->Init(device_);
		it->Set(this,i);
		palette.Set(PostEffectType(i),it->useTextures,
			it->WathchShaderSetIndex(), it->WatchAddessContainer(), it->blendMode);
	}



	ChangePostEffection(PostEffectType::kNone);
}

OffScreen* OffScreenManager::SetEffectSysytem(PostEffectType type_,
	std::unique_ptr<OffScreen> dst_)
{
	effectContainer[type_] = std::move(dst_);
	return effectContainer[type_].get();
}

PostEffectType OffScreenManager::ChangePostEffection(PostEffectType type_)
{
	auto typeBuffer = curPostEffectType;
	palette.ChangeType(type_);
	curPostEffectType = type_;

	return typeBuffer;
}

void OffScreenManager::Update()
{
	effectContainer[curPostEffectType]->Update();
}

PostEffectType OffScreenManager::WatchEffectType()
{
	return curPostEffectType;
}