#pragma once
#include <d3d12.h>
#include "../pipelineComponents.h"

class BlendDescCreator
{
public:
	[[nodiscard]] D3D12_BLEND_DESC CreateBlendDesc(BlendMode blendMode_);

};



