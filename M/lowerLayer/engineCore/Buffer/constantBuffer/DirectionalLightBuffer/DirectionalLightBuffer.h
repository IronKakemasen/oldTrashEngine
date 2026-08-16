#pragma once
#include "../bufferAndMap.h"
#include "../../../Light/DirectionalLight/DirectionalLight.h"

struct DirectionalLightBuffer
{
	BufferAndMap<LightBasePara> dirLight;
};

