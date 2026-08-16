#pragma once
#include <d3d12.h>
#include "../pipelineComponents.h"

class DepthStencilDescCreator
{
public:
	[[nodiscard]] D3D12_DEPTH_STENCIL_DESC CreateDepthStencilDesc(BlendMode blendMode_);

};

