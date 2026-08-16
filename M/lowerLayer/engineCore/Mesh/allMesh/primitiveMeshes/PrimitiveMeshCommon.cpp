#include "PrimitiveMeshCommon.h"
#include "../../../PSO/allPipelineSet.h"
#include <assert.h>

void PrimitiveMeshCommon::Init(uint16_t kMaxDraw_, UINT vertexCnt_, UINT indexCnt_, AllPipelineSet* allPipelineset_)
{
	vertexCnt = vertexCnt_;
	indexCnt = indexCnt_;
	materialBuffer.resize(kMaxDraw_);
	worldMatrixBuffer.resize(kMaxDraw_);
	wvpMatrixBuffer.resize(kMaxDraw_);
	kMaxDraw = kMaxDraw_;

	CreatePSO(allPipelineset_);

}

void PrimitiveMeshCommon::DrawIndexReset()
{
	cur_drawIndex = 0;
}

void PrimitiveMeshCommon::DetectOverDrawing()
{
	assert(cur_drawIndex < kMaxDraw);
}

void PrimitiveMeshCommon::CreatePSO(AllPipelineSet* allPipelineset_)
{
	static bool onlyOnce = true;

	if (onlyOnce)
	{
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

		auto rootparameterFunc = []() {

			std::vector<D3D12_ROOT_PARAMETER> meters;

			meters.emplace_back(RootSignatureCreator::GetRootparameterSRV(ShaderStage::PS, 0));
			meters.emplace_back(RootSignatureCreator::GetRootparameterSRV(ShaderStage::PS, 1));

			meters.emplace_back(RootSignatureCreator::GetRootparameterCBV(ShaderStage::VS, 0));
			meters.emplace_back(RootSignatureCreator::GetRootparameterCBV(ShaderStage::VS, 1));
			meters.emplace_back(RootSignatureCreator::GetRootparameterCBV(ShaderStage::PS, 2));
			meters.emplace_back(RootSignatureCreator::GetRootparameterCBV(ShaderStage::PS, 3));
			meters.emplace_back(RootSignatureCreator::GetRootparameterCBV(ShaderStage::PS, 4));
			meters.emplace_back(RootSignatureCreator::GetRootparameterCBV(ShaderStage::PS, 5));

			return meters;
		};


		auto rootparameterSprite = []() {

			std::vector<D3D12_ROOT_PARAMETER> meters;

			meters.emplace_back(RootSignatureCreator::GetRootparameterSRV(ShaderStage::PS, 0));

			meters.emplace_back(RootSignatureCreator::GetRootparameterCBV(ShaderStage::VS, 0));
			meters.emplace_back(RootSignatureCreator::GetRootparameterCBV(ShaderStage::PS, 1));

			return meters;
			};


		std::string folderPath = "MobilePrimitive/";
		std::string folderPathSprite = "Sprite/";

		allPipelineset_->CreateNewPipeline(folderPath, "MobilePrimitiveMesh.VS", "MobilePrimitiveMesh.PS", inputLayOutFunc, rootparameterFunc);
		allPipelineset_->CreateNewPipeline(folderPathSprite, "Sprite.VS", "Sprite.PS", inputLayOutFunc, rootparameterSprite);

	}
}