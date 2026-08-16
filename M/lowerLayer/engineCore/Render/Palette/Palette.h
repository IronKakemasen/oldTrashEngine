#pragma once
#include "PostEffectType.h"
#include "../../utilities/vectorAndMatrix/vectorAndMatrix.h"
#include <unordered_map>
#include <d3d12.h>
#include "../../PSO/pipelineCreators/pipelineComponents.h"

class OriginalScreen;
class Palette
{
	class Para
	{
	public:

		BlendMode blendMode;
		int shaderSetIndex;
		std::vector<uint16_t> offscreenTextureContainer;
		std::vector<D3D12_GPU_VIRTUAL_ADDRESS> cBufferAddressContainer;
		Para();
	};

	OriginalScreen* originalScreen = nullptr;

public:

	std::unordered_map < PostEffectType, Para> paraContainer;

	PostEffectType curType;

	void Init(OriginalScreen* originalScreen_);
	Palette();
	int WatchShaderSetIndex();
	std::vector<uint16_t> WatchUseTexture();
	std::vector<D3D12_GPU_VIRTUAL_ADDRESS> WatchCBufferAddressContainer();
	inline BlendMode WatchBlendMode()
	{
		return paraContainer[curType].blendMode;
	}

	void Set(PostEffectType type_, std::vector<uint16_t> textures_, 
		int shaderSetIndex_, std::vector<D3D12_GPU_VIRTUAL_ADDRESS> cBufferAddressContainer_,
		BlendMode blendMode_);
	void ChangeType(PostEffectType type_);
};

