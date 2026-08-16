#pragma once
#include <wrl.h>
#include <d3d12.h>


struct VertexBuffer
{
	// 頂点バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> buffer;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW view{};

	void Create(ID3D12Device* device_, UINT sizeOfBuffer_);
};

struct VertexBufferForLine
{
	// 頂点バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> buffer;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW view{};

	void Create(ID3D12Device* device_, UINT sizeOfBuffer_);
};

