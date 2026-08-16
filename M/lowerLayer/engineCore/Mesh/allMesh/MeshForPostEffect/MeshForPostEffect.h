#pragma once
#include <stdint.h>
#include <wrl.h>
#include <d3d12.h>
#include <memory>
#include <assert.h>
#include <vector>

#include "../../meshStructure/Vertex/Vertex.h"
#include "../../../Buffer/constantBuffer/MatrixBuffer/MatrixBuffer.h"
#include "../../../Buffer/constantBuffer/MaterialBuffer/MaterialBuffer.h"
#include "../../../Buffer/VertexBuffer/VertexBuffer.h"
#include "../../../Buffer/IndexBuffer/IndexBuffer.h"

class AllPipelineSet;

struct MeshForPostEffect
{
	int const kMax = 10;
	int const kIndexCnt = 6;
	int const kVertexCnt = 4;
	int curIndex = 0;

	// 頂点バッファマップ
	Vertex* vertexMap = nullptr;
	// インデックスバッファマップ
	uint32_t* indexMap = nullptr;
	//WVPマトリクスバッファ
	std::vector<MatrixBuffer> wvpMatrixBuffer;

protected:
	// 頂点バッファとそのびゅー
	VertexBuffer vertexBuffer;
	// インデックスバッファとそのびゅー
	IndexBuffer indexBuffer;

public:

	MeshForPostEffect(ID3D12Device* device_, AllPipelineSet* allPipelineset_);

	auto* Getter_VertexBufferView()
	{
		return &vertexBuffer.view;
	}

	auto* Getter_IndexBufferView()
	{
		return &indexBuffer.view;
	}

private:
	void CreateMesh(ID3D12Device* device_);
	void CreatePSO(AllPipelineSet* allPipelineset_);

};
