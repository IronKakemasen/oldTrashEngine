#include "inputLayoutDescCreator.h"
#include "../../allPipelineSet.h"
#include <assert.h>

#pragma comment(lib,"d3d12.lib")


void InputLayoutDescCreator::CopyFromIndex(int funcIndex_)
{
	std::vector<D3D12_INPUT_ELEMENT_DESC> tmpDescs = funcs_inputElementDescsCreate[funcIndex_]();
	heap_inputElementDescs.second = UINT(tmpDescs.size());
	heap_inputElementDescs.first = new D3D12_INPUT_ELEMENT_DESC[heap_inputElementDescs.second];


	int index = 0;
	for (auto itr = tmpDescs.begin(); itr != tmpDescs.end(); ++itr,index++)
	{
		heap_inputElementDescs.first[index] = (*itr);
		
	}

}

void InputLayoutDescCreator::AddToFuncs_InputElementDescsCreate(
	std::function<std::vector<D3D12_INPUT_ELEMENT_DESC>()> func_)
{
	sum_created++;
	if (sum_created > AllPipelineSet::kNumShaderSet) assert(false);
	
	funcs_inputElementDescsCreate.emplace_back(func_);

}

void InputLayoutDescCreator::SetInputElementDescs(int funcIndex_)
{
	CopyFromIndex(funcIndex_);
}

D3D12_INPUT_ELEMENT_DESC InputLayoutDescCreator::GetInputElementDesc(
	LPCSTR semanticName_,
	UINT semanticIndex_,
	DXGI_FORMAT format_,
	UINT alignedByteOffset_
)
{
	D3D12_INPUT_ELEMENT_DESC ret_elementDesc = {};

	ret_elementDesc.SemanticName = semanticName_;
	ret_elementDesc.SemanticIndex = semanticIndex_;
	ret_elementDesc.Format = format_;
	ret_elementDesc.AlignedByteOffset = alignedByteOffset_;

	return ret_elementDesc;
}

[[nodiscard]] D3D12_INPUT_LAYOUT_DESC InputLayoutDescCreator::CreateInputLayoutDesc(int funcIndex_)
{
	D3D12_INPUT_LAYOUT_DESC ret_inputLayoutDesc = {};
	SetInputElementDescs(funcIndex_);

	ret_inputLayoutDesc.pInputElementDescs = heap_inputElementDescs.first;
	ret_inputLayoutDesc.NumElements = heap_inputElementDescs.second;;


	return ret_inputLayoutDesc;
}


void InputLayoutDescCreator::SafeRelease()
{
	if (heap_inputElementDescs.first)
	{
		delete[] heap_inputElementDescs.first;
		heap_inputElementDescs.second = 0;
	}
}


