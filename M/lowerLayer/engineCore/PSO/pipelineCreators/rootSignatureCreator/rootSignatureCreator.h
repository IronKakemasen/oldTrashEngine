#pragma once
#include <d3d12.h>
#include <wrl.h>
#include "../pipelineComponents.h"
#include <utility>
#include <string>
#include <functional>
#include <vector>
#include <memory>
#include "../../../Render/Appearance/Appearance.h"

enum ShaderStage
{
    ALL = 0,     //!< 全ステージ.
    VS = 1,     //!< 頂点シェーダ.
    HS = 2,     //!< ハルシェーダ.
    DS = 3,     //!< ドメインシェーダ.
    GS = 4,     //!< ジオメトリシェーダ.
    PS = 5,     //!< ピクセルシェーダ.
};

enum SamplerState
{
    PointWrap,          //!< ポイントサンプリング - 繰り返し.
    PointClamp,         //!< ポイントサンプリング - クランプ.
    LinearWrap,         //!< トライリニアサンプリング - 繰り返し.
    LinearClamp,        //!< トライリニアサンプリング - クランプ.
    AnisotropicWrap,    //!< 異方性サンプリング - 繰り返し.
    AnisotropicClamp,   //!< 異方性サンプリング - クランプ.
};



class RootSignatureCreator
{
	struct DescriptoRangeArray
	{
		D3D12_DESCRIPTOR_RANGE range[1]{};
	};

	int sum_created = 0;

	D3D12_STATIC_SAMPLER_DESC* heap_staticSamplers = nullptr;
	static inline std::vector < std::unique_ptr < DescriptoRangeArray > > descriptorRanges;

	std::vector<std::function<std::vector<D3D12_ROOT_PARAMETER>()>> funcs_rootParametersCreate;
	std::pair<D3D12_ROOT_PARAMETER*, UINT> heap_rootParametersAndSum;

	void CopyFromIndex(int funcIndex_);
	void SetRootParameters(int index_);
	D3D12_STATIC_SAMPLER_DESC CreateStaticSmp(ShaderStage shaderStage_, uint32_t registerNum_, SamplerState state);

public:

	RootSignatureCreator();
	~RootSignatureCreator();

	static D3D12_ROOT_PARAMETER GetRootparameterCBV(ShaderStage stage_, int registerNum_);
	static D3D12_ROOT_PARAMETER GetRootparameterSRV(ShaderStage stage_, int registerNum_);

	void AddToFuncs_RootParametersCreate(std::function<std::vector<D3D12_ROOT_PARAMETER>()> func_);

	[[nodiscard]] Microsoft::WRL::ComPtr<ID3D12RootSignature >CreateRootSignature(
		ID3D12Device* device_, int index_, std::string shaderSetName_);

};
