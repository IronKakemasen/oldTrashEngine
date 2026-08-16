#pragma once
#include <d3d12.h>
#include "../pipelineComponents.h"
#include <string>
#include <vector>
#include <functional>


class InputLayoutDescCreator
{	
	int sum_created = 0;

	std::pair<D3D12_INPUT_ELEMENT_DESC*, UINT> heap_inputElementDescs = { nullptr ,0};
	std::vector < std::function<std::vector<D3D12_INPUT_ELEMENT_DESC>()>> funcs_inputElementDescsCreate;

	//いつか修正
	void SetInputElementDescs(int funcIndex_);
	void CopyFromIndex(int funcIndex_);

public:

	static D3D12_INPUT_ELEMENT_DESC GetInputElementDesc(
		LPCSTR semanticName_,
		UINT semanticIndex_,
		DXGI_FORMAT format_,
		UINT alignedByteOffset_
	);

	void AddToFuncs_InputElementDescsCreate(std::function<std::vector<D3D12_INPUT_ELEMENT_DESC>()> func_);

	[[nodiscard]] D3D12_INPUT_LAYOUT_DESC CreateInputLayoutDesc(int funcIndex_);
	void SafeRelease();
};

