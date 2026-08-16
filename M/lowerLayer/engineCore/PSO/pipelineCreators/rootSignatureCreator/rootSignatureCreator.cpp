#include "rootSignatureCreator.h"
#include "../../../WinApp.h"
#include "../../allPipelineSet.h"

#include <assert.h>

#pragma comment(lib,"d3d12.lib")

RootSignatureCreator::RootSignatureCreator()
{
	heap_staticSamplers = new D3D12_STATIC_SAMPLER_DESC[Appearance::kCount];
}

RootSignatureCreator::~RootSignatureCreator()
{
	delete[] heap_staticSamplers;
}

D3D12_STATIC_SAMPLER_DESC RootSignatureCreator::CreateStaticSmp(ShaderStage shaderStage_, 
	uint32_t registerNum_, SamplerState state)
{
	D3D12_STATIC_SAMPLER_DESC desc = {};

	desc.MipLODBias = D3D12_DEFAULT_MIP_LOD_BIAS;
	desc.MaxAnisotropy = 1;
	desc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	desc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	desc.MinLOD = 0.0f;
	desc.MaxLOD = D3D12_FLOAT32_MAX;
	desc.ShaderRegister = registerNum_;
	desc.RegisterSpace = 0;
	desc.ShaderVisibility = D3D12_SHADER_VISIBILITY(shaderStage_);

	switch (state)
	{
	case SamplerState::PointWrap:
	{
		desc.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
		desc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		desc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		desc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	}
	break;

	case SamplerState::PointClamp:
	{
		desc.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
		desc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		desc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		desc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	}
	break;

	case SamplerState::LinearWrap:
	{
		desc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
		desc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		desc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		desc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	}
	break;

	case SamplerState::LinearClamp:
	{
		desc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
		desc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		desc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		desc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	}
	break;

	case SamplerState::AnisotropicWrap:
	{
		desc.Filter = D3D12_FILTER_ANISOTROPIC;
		desc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		desc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		desc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		desc.MaxAnisotropy = D3D12_MAX_MAXANISOTROPY;
	}
	break;

	case SamplerState::AnisotropicClamp:
	{
		desc.Filter = D3D12_FILTER_ANISOTROPIC;
		desc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		desc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		desc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		desc.MaxAnisotropy = D3D12_MAX_MAXANISOTROPY;
	}
	break;
	}
	return desc;
}


void RootSignatureCreator::CopyFromIndex(int funcIndex_)
{
	std::vector<D3D12_ROOT_PARAMETER> rootParameters = funcs_rootParametersCreate[funcIndex_]();
	heap_rootParametersAndSum.second = UINT(rootParameters.size());
	heap_rootParametersAndSum.first = new D3D12_ROOT_PARAMETER[heap_rootParametersAndSum.second];

	int index = 0;
	for (auto itr = rootParameters.begin(); itr != rootParameters.end(); ++itr, index++)
	{
		heap_rootParametersAndSum.first[index] = (*itr);
	}
}

void RootSignatureCreator::AddToFuncs_RootParametersCreate(std::function<std::vector<D3D12_ROOT_PARAMETER>()> func_)
{
	sum_created++;
	if (sum_created > AllPipelineSet::kNumShaderSet) assert(false);

	funcs_rootParametersCreate.emplace_back(func_);
}

void RootSignatureCreator::SetRootParameters(int index_)
{
	CopyFromIndex(index_);
}

D3D12_ROOT_PARAMETER RootSignatureCreator::GetRootparameterCBV(ShaderStage stage_, int registerNum_)
{
	D3D12_ROOT_PARAMETER ret_rootParameter = {};

	ret_rootParameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	//PixcelShaderで使う
	ret_rootParameter.ShaderVisibility = D3D12_SHADER_VISIBILITY(stage_);
	//レジスタ番号
	ret_rootParameter.Descriptor.ShaderRegister = registerNum_;

	return ret_rootParameter;
}

D3D12_ROOT_PARAMETER RootSignatureCreator::GetRootparameterSRV(ShaderStage stage_, int registerNum_)
{
	D3D12_ROOT_PARAMETER ret_rootParameter = {};
	std::unique_ptr < RootSignatureCreator::DescriptoRangeArray > dst;
	dst.reset(new RootSignatureCreator::DescriptoRangeArray);

	dst->range[0].BaseShaderRegister = registerNum_;
	//数は1つ
	dst->range[0].NumDescriptors = 1;
	//SRVを使う
	dst->range[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	//offsetを自動計算
	dst->range[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	//Descriptortableを使う
	ret_rootParameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	//pixcelShaderを使う
	ret_rootParameter.ShaderVisibility = D3D12_SHADER_VISIBILITY(stage_);
	//tableの中身の配列を指定
	ret_rootParameter.DescriptorTable.pDescriptorRanges = dst->range;
	//tableで利用する
	ret_rootParameter.DescriptorTable.NumDescriptorRanges = 1;

	RootSignatureCreator::descriptorRanges.emplace_back(std::move(dst));

	return ret_rootParameter;
}

[[nodiscard]] Microsoft::WRL::ComPtr<ID3D12RootSignature> RootSignatureCreator::CreateRootSignature(
	ID3D12Device* device_,
	int index_,
	std::string shaderSetName_)
{
	Microsoft::WRL::ComPtr<ID3D12RootSignature> ret_rootSignature = nullptr;

	D3D12_ROOT_SIGNATURE_DESC signatureDesc = {};

	//バイリニアフィルター
	heap_staticSamplers[Appearance::kColormap] = 
		CreateStaticSmp(ShaderStage::PS, 0, SamplerState::LinearWrap);
	heap_staticSamplers[Appearance::kNormalmap] = 
		CreateStaticSmp(ShaderStage::PS, 1, SamplerState::AnisotropicWrap);
	heap_staticSamplers[Appearance::kSpecularMap] = 
		CreateStaticSmp(ShaderStage::PS, 2, SamplerState::LinearWrap);
	heap_staticSamplers[Appearance::kShininessMap] = 
		CreateStaticSmp(ShaderStage::PS, 3, SamplerState::PointWrap);

	signatureDesc.pStaticSamplers = heap_staticSamplers;
	signatureDesc.NumStaticSamplers = UINT(Appearance::kCount);

	SetRootParameters(index_);

	//ルートパラメータ配列へのポインタ
	signatureDesc.pParameters = heap_rootParametersAndSum.first;
	//配列の長さ
	signatureDesc.NumParameters = heap_rootParametersAndSum.second;
	//pixcelShaderで読むConstantBufferのBind情報を追加する
	signatureDesc.Flags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	Microsoft::WRL::ComPtr<ID3DBlob> signatureBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;
	 
	//シリアライズしてバイナリにする
	HRESULT hr = D3D12SerializeRootSignature(
		&signatureDesc,
		D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	if (FAILED(hr))
	{
		Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));

		assert(false);
	}

	//バイナリをもとにrootSignatureを作成
	hr = device_->CreateRootSignature(0,
		signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(),
		IID_PPV_ARGS(ret_rootSignature.GetAddressOf()));

	assert(SUCCEEDED(hr));
	Log(WinApp::log, shaderSetName_+" : Complete create rootSignature\n");

	//解放
	delete[] heap_rootParametersAndSum.first;

	return ret_rootSignature;
}


