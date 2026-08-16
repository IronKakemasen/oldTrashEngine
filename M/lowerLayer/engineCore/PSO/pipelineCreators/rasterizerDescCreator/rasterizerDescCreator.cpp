#include "rasterizerDescCreator.h"
#pragma comment(lib,"d3d12.lib")


[[nodiscard]] D3D12_RASTERIZER_DESC RasterizerDescCreator::CreateRasterizerDesc(CullMode cullMode_)
{
	D3D12_RASTERIZER_DESC ret_rasterizerDesc{};

	//カルモード
	ret_rasterizerDesc.CullMode = D3D12_CULL_MODE((int)cullMode_ + 1);

	//三角形の中身を塗りつぶす
	ret_rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
	ret_rasterizerDesc.FrontCounterClockwise = FALSE;
	ret_rasterizerDesc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
	ret_rasterizerDesc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
	ret_rasterizerDesc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
	ret_rasterizerDesc.DepthClipEnable = FALSE;
	ret_rasterizerDesc.MultisampleEnable = FALSE;
	ret_rasterizerDesc.AntialiasedLineEnable = FALSE;
	ret_rasterizerDesc.ForcedSampleCount = 0;
	ret_rasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;


	return ret_rasterizerDesc;
}

