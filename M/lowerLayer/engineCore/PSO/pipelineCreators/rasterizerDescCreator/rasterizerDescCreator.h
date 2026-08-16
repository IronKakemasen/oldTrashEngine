#pragma once
#include <d3d12.h>
#include "../pipelineComponents.h"

class RasterizerDescCreator
{
public:
	[[nodiscard]] D3D12_RASTERIZER_DESC CreateRasterizerDesc(CullMode cullMode_);

};

