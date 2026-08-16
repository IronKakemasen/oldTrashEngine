#pragma once
#include <wrl.h>
#include <d3d12.h>



struct IndexBuffer
{
	// インデックスバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> buffer;
	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW view{};

	void Create(ID3D12Device* device_, UINT sizeOfBuffer_);

};

