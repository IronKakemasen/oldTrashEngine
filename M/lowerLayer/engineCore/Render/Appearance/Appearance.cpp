#include "Appearance.h"
#include <assert.h>

Appearance::Appearance()
{
	color = { 255,255,255,255 };
	cullMode = kCullModeBack;
	blendMode = kBlendModeNormal;
	shaderSetIndex = 0;
	doDraw = true;
	use_texHandles.resize(1);
	use_texHandles[0] = 0;		//(White2x2)
	texHandlesContainer[kColormap] = 0;		//(White2x2)
	texHandlesContainer[kNormalmap] = kNone;		
	texHandlesContainer[kSpecularMap] = kNone;		
	texHandlesContainer[kShininessMap] = kNone;		
	roughness = 1.0f;
	metalic = 0.75f;
}

Vector4 Appearance::GetComplementaryColor(Vector4 color_)
{
	return { 255 - color_.x, 255 - color_.y, 255 - color_.z, 255.0f };
}


void Appearance::SetUsingTextureFromContainer(bool colorMap_, bool normalMap_,
	bool metalicMap_, bool roughnessMap_)
{
	bool useArray[Appearance::kCount] =
	{
		colorMap_,normalMap_,metalicMap_,roughnessMap_
	};

	use_texHandles.clear();

	for (int i = 0; i < Appearance::kCount; ++i)
	{
		if (!useArray[i]) continue;

		if (texHandlesContainer[Appearance::TextureType(i)] == (int)kNone)
		{
			assert(false);
		}

		use_texHandles.emplace_back(texHandlesContainer[Appearance::TextureType(i)]);
	}

}



