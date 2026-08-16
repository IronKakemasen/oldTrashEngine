#include "depthStencilDescCreator.h"
#pragma comment(lib,"d3d12.lib")

[[nodiscard]] D3D12_DEPTH_STENCIL_DESC DepthStencilDescCreator::CreateDepthStencilDesc(BlendMode blendMode_)
{
	D3D12_DEPTH_STENCIL_DESC ret_depthStencilDesc{};

	ret_depthStencilDesc.DepthEnable = true;

	//書き込みする
	if (blendMode_ == kBlendModeNormal)
	{
		ret_depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	}

	else if (blendMode_ == kBlendModeAdd)
	{
		ret_depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	}

	//比較関数はLessEqual。つかり近ければ描画される
	//ret_depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	ret_depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_GREATER_EQUAL;



	return ret_depthStencilDesc;
}
