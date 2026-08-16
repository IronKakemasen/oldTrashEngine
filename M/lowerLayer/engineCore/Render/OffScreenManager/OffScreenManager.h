#pragma once
#include "./PostEffects/PostEffects.h"
#include "../Palette/Palette.h"

#include <unordered_map>
#include <memory>

class TextureDataManager;
class ExclusiveDraw;
class CommandControl;
class SwapChainControl;
struct SrvDescriptorHeap;

class DOF_Screen :public OffScreen
{

public:

	virtual void Init(ID3D12Device* device_)override;
	virtual void Update()override;
	DOF_Screen(TextureDataManager* textureDataManager_, int numRequired_, OriginalScreen* originalScreen_, float width_, float height_);
};


class OffScreenManager
{
	std::unique_ptr<OriginalScreen> originalScreen;
	ExclusiveDraw* exclusiveDraw = nullptr;
	TextureDataManager* textureDataManager = nullptr;
	std::unordered_map<PostEffectType, std::unique_ptr<OffScreen> >
		effectContainer;

	OffScreen* SetEffectSysytem(PostEffectType type_,
		std::unique_ptr<OffScreen> dst_);

	Palette palette;

	PostEffectType curPostEffectType;
	CommandControl* commandControl;
	SwapChainControl* swapChainControl;
	SrvDescriptorHeap* srvDescriptorHeap;
	int preRenderTargetStat = {};

	void PostEffectBegin(PostEffectBuffer* srcBuffer_, PostEffectType postEffectType_);
	void PostEffectEnd(PostEffectBuffer* srcBuffer_);

public:
	static inline std::unique_ptr<DOF_Screen> dOF_Screen;

	static inline int curRenderTargetStatus = 0;
	inline OriginalScreen* GetOriginalScreen() { return originalScreen.get(); }
	inline Palette* GetPalette() { return &palette; }
	void Init(TextureDataManager* textureDataManager_, 
		ExclusiveDraw* exclusiveDraw_, ID3D12Device* device_,
		CommandControl* commandControl_, SwapChainControl* swapChainControl_, SrvDescriptorHeap* srvDescriptorHeap_);
	PostEffectType ChangePostEffection(PostEffectType type_);
	void Update();
	PostEffectType WatchEffectType();
	void DrawOnResource(PostEffectBuffer* srcBuffer_, PostEffectType postEffectType_);


};

