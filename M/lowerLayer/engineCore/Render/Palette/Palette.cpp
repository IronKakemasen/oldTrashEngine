#include "Palette.h"
#include "../../../commonVariables.h"
#include "../OriginalScreen/OriginalScreen.h"

Palette::Para::Para()
{
	shaderSetIndex = 0;
}

void Palette::Set(PostEffectType type_, std::vector<uint16_t> textures_,
	int shaderSetIndex_, std::vector<D3D12_GPU_VIRTUAL_ADDRESS> cBufferAddressContainer_,
	BlendMode blendMode_)
{
	paraContainer[type_].offscreenTextureContainer = textures_;
	paraContainer[type_].shaderSetIndex = shaderSetIndex_;
	paraContainer[type_].cBufferAddressContainer = cBufferAddressContainer_;
	paraContainer[type_].blendMode = blendMode_;

	if (textures_.empty())
	{
		paraContainer[type_].offscreenTextureContainer =
			originalScreen->WatchTextureIndexes();
	}
}

void Palette::ChangeType(PostEffectType type_)
{
	curType = type_;
}


void Palette::Init(OriginalScreen* originalScreen_)
{
	originalScreen = originalScreen_;
}

Palette::Palette()
{
	curType = PostEffectType::kNone;
}

int Palette::WatchShaderSetIndex()
{
	return paraContainer[curType].shaderSetIndex;
}

std::vector<uint16_t> Palette::WatchUseTexture()
{
	return paraContainer[curType].offscreenTextureContainer;
}

std::vector<D3D12_GPU_VIRTUAL_ADDRESS> Palette::WatchCBufferAddressContainer()
{
	return paraContainer[curType].cBufferAddressContainer;
}
