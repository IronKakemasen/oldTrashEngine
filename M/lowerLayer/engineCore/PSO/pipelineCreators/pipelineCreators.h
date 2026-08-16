#pragma once
#include "./rootSignatureCreator/rootSignatureCreator.h"
#include "./inputLayoutDescCreator/inputLayoutDescCreator.h"
#include "./blendDescCreator/blendDescCreator.h"
#include "./rasterizerDescCreator/rasterizerDescCreator.h"
#include "./depthStencilDescCreator/depthStencilDescCreator.h"


class PipelineCreators
{
public:
	RootSignatureCreator rootSignatureCreator;
	InputLayoutDescCreator inputLayoutCreator;
	BlendDescCreator blendDescCreator;
	RasterizerDescCreator rasterizerDescCreator;
	DepthStencilDescCreator depthStencilDescCreator;

	void SafeReset();
};
