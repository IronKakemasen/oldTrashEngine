#include "blendDescCreator.h"
#pragma comment(lib,"d3d12.lib")
#include "../../pipelineSet/pipelineSet.h"


[[nodiscard]] D3D12_BLEND_DESC BlendDescCreator::CreateBlendDesc(BlendMode blendMode_)
{
	D3D12_BLEND_DESC ret_blendDesc{};
	ret_blendDesc.IndependentBlendEnable = FALSE;
	ret_blendDesc.AlphaToCoverageEnable = FALSE;

	ret_blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	ret_blendDesc.RenderTarget[0].BlendEnable = TRUE;
	ret_blendDesc.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
	ret_blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	ret_blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	ret_blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;


	switch (blendMode_)
	{
	case kBlendModeNormal:

		ret_blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
		ret_blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		ret_blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		break;

	case kBlendModeAdd:
		ret_blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
		ret_blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		ret_blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
		break;
	}

	if (PipelineSet::numRenderTaget == 2)
	{
		ret_blendDesc.RenderTarget[1].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
		ret_blendDesc.RenderTarget[1].BlendEnable = TRUE;
		ret_blendDesc.RenderTarget[1].LogicOp = D3D12_LOGIC_OP_NOOP;
		ret_blendDesc.RenderTarget[1].SrcBlendAlpha = D3D12_BLEND_ONE;
		ret_blendDesc.RenderTarget[1].BlendOpAlpha = D3D12_BLEND_OP_ADD;
		ret_blendDesc.RenderTarget[1].DestBlendAlpha = D3D12_BLEND_ZERO;
		ret_blendDesc.RenderTarget[1].SrcBlend = D3D12_BLEND_SRC_ALPHA;
		ret_blendDesc.RenderTarget[1].BlendOp = D3D12_BLEND_OP_ADD;
		ret_blendDesc.RenderTarget[1].DestBlend = D3D12_BLEND_ONE;
	}



	return ret_blendDesc;
}
