#include "allPipelineSet.h"
#include "../../../utilities/benriTemplateFunc/benriTempFunc.h"
#include <assert.h>
#include "../VpShader/vpShaders.h"

PipelineSet* AllPipelineSet::Getter_pipelineSet(int shaderSetIndex_, int blendMode_, int cullMode_, int atoduke_)
{
	auto* p = pipelineSets[shaderSetIndex_][blendMode_][cullMode_][atoduke_].get();

	return p;
}


void AllPipelineSet::Initialize(ID3D12Device* device_, VpShaders* vpShaders_, ID3D12GraphicsCommandList* commandList_)
{
	commandList = commandList_;
	vpShaders = vpShaders_;
	device = device_;

}

void AllPipelineSet::CreateNewPipeline(
	std::string folderPath_,
	std::string vsFileName_,
	std::string psFileName_,
	std::function<std::vector<D3D12_INPUT_ELEMENT_DESC>()> inputElementDescCreateFunc_,
	std::function<std::vector<D3D12_ROOT_PARAMETER>()> rootParameterCreateFunc_,
	bool isTopologyLine_)
{
	std::string tableName = vsFileName_ + " + " + psFileName_;

	vpShaders->AddPixelShader(folderPath_,psFileName_);
	vpShaders->AddVertexShader(folderPath_,vsFileName_);
	vpShaders->AddToTable(tableName, psFileName_, vsFileName_);
	pipelineSets[0][0][0][0]->Activate_InputLayoutCreateFunc(inputElementDescCreateFunc_);
	pipelineSets[0][0][0][0]->Activate_RootparameterCreateFunc(rootParameterCreateFunc_);

	Add(tableName, isTopologyLine_);

}

void AllPipelineSet::Add(std::string shaderSetName_, bool isTopologyLine)
{
	if (vpShaders)
	{
		auto* vpShaderTable = vpShaders->Getter_VPShaderTable();

		assert(DoesContain(vpShaderTable->Getter_NameList(), shaderSetName_));

		int const shaderSetIndex = vpShaderTable->Getter_Map_nameAndID()[shaderSetName_];

		pipelineSets[shaderSetIndex][kBlendModeNormal][kCullModeBack][0] =
			PipelineSet::CreateGraphicsPipelineSet(device, shaderSetName_, vpShaderTable,
				kBlendModeNormal, CullMode::kCullModeBack, isTopologyLine,0);

		pipelineSets[shaderSetIndex][kBlendModeNormal][kCullModeBack][1] =
			PipelineSet::CreateGraphicsPipelineSet(device, shaderSetName_, vpShaderTable,
				kBlendModeNormal, CullMode::kCullModeBack, isTopologyLine, 1);

		pipelineSets[shaderSetIndex][kBlendModeNormal][kCullModeFront][0] =
			PipelineSet::CreateGraphicsPipelineSet(device, shaderSetName_, vpShaderTable,
				kBlendModeNormal, CullMode::kCullModeFront, isTopologyLine, 0);

		pipelineSets[shaderSetIndex][kBlendModeNormal][kCullModeFront][1] =
			PipelineSet::CreateGraphicsPipelineSet(device, shaderSetName_, vpShaderTable,
				kBlendModeNormal, CullMode::kCullModeFront, isTopologyLine, 1);

		pipelineSets[shaderSetIndex][kBlendModeNormal][kCullModeNone][0] =
			PipelineSet::CreateGraphicsPipelineSet(device, shaderSetName_, vpShaderTable,
				kBlendModeNormal, CullMode::kCullModeNone, isTopologyLine, 0);

		pipelineSets[shaderSetIndex][kBlendModeNormal][kCullModeNone][1] =
			PipelineSet::CreateGraphicsPipelineSet(device, shaderSetName_, vpShaderTable,
				kBlendModeNormal, CullMode::kCullModeNone, isTopologyLine, 1);


		pipelineSets[shaderSetIndex][kBlendModeAdd][kCullModeBack][0] =
			PipelineSet::CreateGraphicsPipelineSet(device, shaderSetName_, vpShaderTable,
				kBlendModeAdd, CullMode::kCullModeBack, isTopologyLine, 0);

		pipelineSets[shaderSetIndex][kBlendModeAdd][kCullModeBack][1] =
			PipelineSet::CreateGraphicsPipelineSet(device, shaderSetName_, vpShaderTable,
				kBlendModeAdd, CullMode::kCullModeBack, isTopologyLine, 1);

		pipelineSets[shaderSetIndex][kBlendModeAdd][kCullModeFront][0] =
			PipelineSet::CreateGraphicsPipelineSet(device, shaderSetName_, vpShaderTable,
				kBlendModeAdd, CullMode::kCullModeFront, isTopologyLine, 0);

		pipelineSets[shaderSetIndex][kBlendModeAdd][kCullModeFront][1] =
			PipelineSet::CreateGraphicsPipelineSet(device, shaderSetName_, vpShaderTable,
				kBlendModeAdd, CullMode::kCullModeFront, isTopologyLine, 1);

		pipelineSets[shaderSetIndex][kBlendModeAdd][kCullModeNone][0] =
			PipelineSet::CreateGraphicsPipelineSet(device, shaderSetName_, vpShaderTable,
				kBlendModeAdd, CullMode::kCullModeNone, isTopologyLine, 0);

		pipelineSets[shaderSetIndex][kBlendModeAdd][kCullModeNone][1] =
			PipelineSet::CreateGraphicsPipelineSet(device, shaderSetName_, vpShaderTable,
				kBlendModeAdd, CullMode::kCullModeNone, isTopologyLine, 1);

		//CommandListSetter
		for (int k = 0; k < kCountOfBlendMode; ++k)
		{
			for (int j = 0; j < kCountOfCullMode; ++j)
			{
				pipelineSets[shaderSetIndex][k][j][0]->Setter_Commandlist(commandList);
				pipelineSets[shaderSetIndex][k][j][1]->Setter_Commandlist(commandList);
			}
		}
	}

	else
	{
		assert(false);
	}
}




// 末尾あたりに追加実装
void AllPipelineSet::Setter_CommandlistForAll(ID3D12GraphicsCommandList* commandList_)
{
	// 内部に保持している pipelineSets 配列の全要素に対して Setter_Commandlist を呼ぶ
	// kCountOfBlendMode, kCountOfCullMode は既存コードと同じ定数を使う想定
	for (size_t shaderIndex = 0; shaderIndex < kNumShaderSet; ++shaderIndex)
	{
		for (int b = 0; b < kCountOfBlendMode; ++b)
		{
			for (int c = 0; c < kCountOfCullMode; ++c)
			{
				for (int a = 0; a < 2; ++a)
				{
					auto& p = pipelineSets[shaderIndex][b][c][a];
					if (p)
					{
						p->Setter_Commandlist(commandList_);
					}
				}
			}
		}
	}
	// 内部保存ポインタも更新しておく
	commandList = commandList_;
}