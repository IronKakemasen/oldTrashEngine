#pragma once
#include "./pipelineSet/pipelineSet.h"
#include <memory>

class VpShaders;

class AllPipelineSet
{
public:
	static int const kNumShaderSet = 40;
	//[ pipelineSet ]
	std::unique_ptr<PipelineSet> pipelineSets[kNumShaderSet][kCountOfBlendMode][kCountOfCullMode][2];

	// フレーム開始時に現在の commandList を全 PipelineSet に設定する
	void Setter_CommandlistForAll(ID3D12GraphicsCommandList* commandList_);

private:

	VpShaders* vpShaders = nullptr;
	ID3D12Device* device = nullptr;
	ID3D12GraphicsCommandList* commandList = nullptr;

	void Add(std::string shaderSetName_, bool isTopologyLine = false);

public:

	void Initialize(ID3D12Device* device_, VpShaders* vpShaders_, ID3D12GraphicsCommandList* commandList_);
	void CreateNewPipeline(
		std::string folderPath_,
		std::string vsFileName_,
		std::string psFileName_,
		std::function<std::vector<D3D12_INPUT_ELEMENT_DESC>()> inputElementDescCreateFunc_,
		std::function<std::vector<D3D12_ROOT_PARAMETER>()> rootParameterCreateFunc_,
		bool isTopologyLine_ = false);

	PipelineSet* Getter_pipelineSet(int shaderSetIndex_, int blendMode_, int cullMode_,int atoduke_ = 0);

};

