#pragma once
#include "../../../../utilities/UVTransform/UVTransform.h"
#include "../../../../utilities/Transform/Transform.h"
#include "../../PSO/pipelineCreators/pipelineComponents.h"
#include <unordered_map>

struct Appearance
{
	enum TextureType
	{
		kColormap,
		kNormalmap,
		kSpecularMap,
		kShininessMap,

		kCount,
		kNone = 999
	};

	Transform trans;
	UVTransform uvTrans;
	Vector4 color;
	float roughness;
	float metalic;
	CullMode cullMode;
	BlendMode blendMode;
	int shaderSetIndex;
	std::unordered_map < TextureType ,int > texHandlesContainer;
	std::vector < int > use_texHandles;
	bool doDraw;

	void SetUsingTextureFromContainer(bool colorMap_, bool normalMap_,
		bool metalicMap_, bool roughnessMap_);

	static Vector4 GetComplementaryColor(Vector4 color_);
	Appearance();
};


