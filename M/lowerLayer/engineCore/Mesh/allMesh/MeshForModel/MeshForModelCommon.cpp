#include "MeshForModelCommon.h"
#include "../../../PSO/allPipelineSet.h"
#include <assert.h>

void MeshAndDataCommon::Batch()
{
	meshForModel;
}

void MeshAndDataCommon::CreatePSO(AllPipelineSet* allPipelineset_)
{
	static bool onlyOnce = true;

	if (!onlyOnce) return;
	onlyOnce = false;

	auto inputLayOutFunc = []()
	{
			std::vector<D3D12_INPUT_ELEMENT_DESC> descs;
			descs.emplace_back(InputLayoutDescCreator::GetInputElementDesc(
				"POSITION",
				0,
				DXGI_FORMAT_R32G32B32_FLOAT,
				D3D12_APPEND_ALIGNED_ELEMENT
			));

			descs.emplace_back(InputLayoutDescCreator::GetInputElementDesc(
				"TEXCOORD",
				0,
				DXGI_FORMAT_R32G32_FLOAT,
				D3D12_APPEND_ALIGNED_ELEMENT
			));

			descs.emplace_back(InputLayoutDescCreator::GetInputElementDesc(
				"NORMAL",
				0,
				DXGI_FORMAT_R32G32B32_FLOAT,
				D3D12_APPEND_ALIGNED_ELEMENT
			));

			descs.emplace_back(InputLayoutDescCreator::GetInputElementDesc(
				"TANGENT",
				0,
				DXGI_FORMAT_R32G32B32_FLOAT,
				D3D12_APPEND_ALIGNED_ELEMENT
			));

			return descs;
	};

	auto dofInput = []()
		{
			std::vector<D3D12_INPUT_ELEMENT_DESC> descs;
			descs.emplace_back(InputLayoutDescCreator::GetInputElementDesc(
				"POSITION",
				0,
				DXGI_FORMAT_R32G32B32_FLOAT,
				D3D12_APPEND_ALIGNED_ELEMENT
			));

			descs.emplace_back(InputLayoutDescCreator::GetInputElementDesc(
				"TEXCOORD",
				0,
				DXGI_FORMAT_R32G32_FLOAT,
				D3D12_APPEND_ALIGNED_ELEMENT
			));

			descs.emplace_back(InputLayoutDescCreator::GetInputElementDesc(
				"NORMAL",
				0,
				DXGI_FORMAT_R32G32B32_FLOAT,
				D3D12_APPEND_ALIGNED_ELEMENT
			));

			descs.emplace_back(InputLayoutDescCreator::GetInputElementDesc(
				"TANGENT",
				0,
				DXGI_FORMAT_R32G32B32_FLOAT,
				D3D12_APPEND_ALIGNED_ELEMENT
			));

			descs.emplace_back(InputLayoutDescCreator::GetInputElementDesc(
				"TEXCOORD",
				1,
				DXGI_FORMAT_R32G32B32_FLOAT,
				D3D12_APPEND_ALIGNED_ELEMENT
			));

			return descs;
		};


	auto modelSimpleRP = []()
	{

		std::vector<D3D12_ROOT_PARAMETER> meters;

		meters.emplace_back(RootSignatureCreator::GetRootparameterSRV(ShaderStage::PS, 0));
		meters.emplace_back(RootSignatureCreator::GetRootparameterSRV(ShaderStage::PS, 1));
		meters.emplace_back(RootSignatureCreator::GetRootparameterSRV(ShaderStage::PS, 2));

		meters.emplace_back(RootSignatureCreator::GetRootparameterCBV(ShaderStage::VS, 0));
		meters.emplace_back(RootSignatureCreator::GetRootparameterCBV(ShaderStage::PS, 1));
		meters.emplace_back(RootSignatureCreator::GetRootparameterCBV(ShaderStage::PS, 2));
		meters.emplace_back(RootSignatureCreator::GetRootparameterCBV(ShaderStage::PS, 3));
		meters.emplace_back(RootSignatureCreator::GetRootparameterCBV(ShaderStage::PS, 4));
		meters.emplace_back(RootSignatureCreator::GetRootparameterCBV(ShaderStage::PS, 5));


		return meters;
	};

	auto modelGGX = []()
		{

			std::vector<D3D12_ROOT_PARAMETER> meters;

			meters.emplace_back(RootSignatureCreator::GetRootparameterSRV(ShaderStage::PS, 0));
			meters.emplace_back(RootSignatureCreator::GetRootparameterSRV(ShaderStage::PS, 1));
			meters.emplace_back(RootSignatureCreator::GetRootparameterSRV(ShaderStage::PS, 2));

			meters.emplace_back(RootSignatureCreator::GetRootparameterCBV(ShaderStage::VS, 0));
			meters.emplace_back(RootSignatureCreator::GetRootparameterCBV(ShaderStage::PS, 1));
			meters.emplace_back(RootSignatureCreator::GetRootparameterCBV(ShaderStage::PS, 2));
			meters.emplace_back(RootSignatureCreator::GetRootparameterCBV(ShaderStage::PS, 3));
			meters.emplace_back(RootSignatureCreator::GetRootparameterCBV(ShaderStage::PS, 4));
			meters.emplace_back(RootSignatureCreator::GetRootparameterCBV(ShaderStage::VS, 5));


			return meters;
		};

	auto modelBumpRP = []() {

		std::vector<D3D12_ROOT_PARAMETER> meters;

		meters.emplace_back(RootSignatureCreator::GetRootparameterSRV(ShaderStage::PS, 0));
		meters.emplace_back(RootSignatureCreator::GetRootparameterSRV(ShaderStage::PS, 1));
		meters.emplace_back(RootSignatureCreator::GetRootparameterSRV(ShaderStage::PS, 2));
		meters.emplace_back(RootSignatureCreator::GetRootparameterSRV(ShaderStage::PS, 3));

		meters.emplace_back(RootSignatureCreator::GetRootparameterCBV(ShaderStage::VS, 0));
		meters.emplace_back(RootSignatureCreator::GetRootparameterCBV(ShaderStage::PS, 1));
		meters.emplace_back(RootSignatureCreator::GetRootparameterCBV(ShaderStage::PS, 2));
		meters.emplace_back(RootSignatureCreator::GetRootparameterCBV(ShaderStage::PS, 3));
		meters.emplace_back(RootSignatureCreator::GetRootparameterCBV(ShaderStage::PS, 4));

		return meters;
	};

	auto modelShaderToy = []() {

		std::vector<D3D12_ROOT_PARAMETER> meters;

		meters.emplace_back(RootSignatureCreator::GetRootparameterCBV(ShaderStage::VS, 0));
		meters.emplace_back(RootSignatureCreator::GetRootparameterCBV(ShaderStage::PS, 1));
		return meters;
		};


	std::string folderPath = "Model/";
	std::string folderPathShaderToy = "ShaderToy/";

	//modelNoLight
	allPipelineset_->CreateNewPipeline(folderPath, "ModelNoLight.VS", "ModelNoLight.PS", inputLayOutFunc, modelSimpleRP);
	//modelSimple
	allPipelineset_->CreateNewPipeline(folderPath, "ModelSimple.VS", "ModelSimple.PS", inputLayOutFunc, modelSimpleRP);
	//modelPhong
	allPipelineset_->CreateNewPipeline(folderPath, "ModelPhong.VS", "ModelPhong.PS", inputLayOutFunc, modelSimpleRP);
	//modelNormalizedPhong
	allPipelineset_->CreateNewPipeline(folderPath, "ModelNormalizedPhong.VS", "ModelNormalizedPhong.PS", inputLayOutFunc, modelSimpleRP);
	//modelBump
	allPipelineset_->CreateNewPipeline(folderPath, "ModelBump.VS", "ModelBump.PS", inputLayOutFunc, modelBumpRP);
	//ModelCookTorrance
	allPipelineset_->CreateNewPipeline(folderPath, "ModelCookTorrance.VS", "ModelCookTorrance.PS", inputLayOutFunc, modelSimpleRP);
	
	PipelineSet::numRenderTaget = 2;
	//ModelGGX
	allPipelineset_->CreateNewPipeline(folderPath, "ModelGGX.VS", "ModelGGX.PS", dofInput, modelGGX);
	allPipelineset_->CreateNewPipeline(folderPath, "kadai0vs", "kadai0", dofInput, modelGGX);
	allPipelineset_->CreateNewPipeline(folderPath, "kadai1vs", "kadai1ps", dofInput, modelGGX);

	PipelineSet::numRenderTaget = 1;

	//shaderToy
	allPipelineset_->CreateNewPipeline(folderPathShaderToy, "tft3RH.VS", "tft3RH.PS", inputLayOutFunc, modelShaderToy);
	allPipelineset_->CreateNewPipeline(folderPathShaderToy, "Mt2SzR.VS", "Mt2SzR.PS", inputLayOutFunc, modelShaderToy);

		
	onlyOnce = false;

}

