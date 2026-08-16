#include "quadMesh.h"

QuadMesh::QuadMesh(AllPipelineSet* allPipelineset_)
{
	Init((int)MaxDraw::kQuad, 4, 6, allPipelineset_);
}

void QuadMesh::CreateMesh(ID3D12Device* device_)
{
	// 頂点データのサイズ
	UINT sizeOfVertexBuffer = 
		static_cast<UINT>(sizeof(Vertex) * vertexCnt * (int)MaxDraw::kQuad);
	// 頂点バッファ生成
	vertexBuffer.Create(device_, sizeOfVertexBuffer);
	// 頂点バッファのマッピング
	HRESULT result = vertexBuffer.buffer->Map(0, nullptr, reinterpret_cast<void**>(&vertexMap));
	assert(SUCCEEDED(result));

	// インデックスデータのサイズ
	UINT sizeOfIndexBuffer = static_cast<UINT>(sizeof(uint32_t) * indexCnt * (int)MaxDraw::kQuad);
	indexBuffer.Create(device_, sizeOfIndexBuffer);

	// インデックスバッファのマッピング
	result = indexBuffer.buffer->Map(0, nullptr, reinterpret_cast<void**>(&indexMap));
	assert(SUCCEEDED(result));

	for (UINT i = 0; i < kMaxDraw; ++i)
	{
		// 行列バッファ生成
		worldMatrixBuffer[i].matrix.CreateAndMapping(device_);
		//wvp行列バッファの生成
		wvpMatrixBuffer[i].matrix.CreateAndMapping(device_);
		// マテリアルバッファ作成
		materialBuffer[i].material.CreateAndMapping(device_);
	}

}


