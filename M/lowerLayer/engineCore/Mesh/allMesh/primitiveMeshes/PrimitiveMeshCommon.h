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

struct PrimitiveMeshCommon
{
	enum class MaxDraw
	{
		kTriangle = 128,
		kQuad = 4000,
	};

	// 頂点バッファマップ
	Vertex* vertexMap = nullptr;
	// インデックスバッファマップ
	uint32_t* indexMap = nullptr;

	//マテリアルバッファ
	std::vector<MaterialBuffer> materialBuffer;
	//ワールドマトリクスバッファ
	std::vector<MatrixBuffer> worldMatrixBuffer;
	//WVPマトリクスバッファ
	std::vector<MatrixBuffer> wvpMatrixBuffer;

	uint16_t cur_drawIndex = 0;
	uint16_t kMaxDraw = 0;
	UINT vertexCnt = 0;
	UINT indexCnt = 0;

protected:
	// 頂点バッファとそのびゅー
	VertexBuffer vertexBuffer;
	// インデックスバッファとそのびゅー
	IndexBuffer indexBuffer;

	virtual void CreateMesh(ID3D12Device* device_) = 0;
	void Init(uint16_t kMaxDraw_,UINT vertexCnt_,UINT indexCnt_, AllPipelineSet* allPipelineset_);

public:

	void DrawIndexReset();
	void DetectOverDrawing();

	auto* Getter_VertexBufferView()
	{
		return &vertexBuffer.view;
	}

	auto* Getter_IndexBufferView()
	{
		return &indexBuffer.view;
	}


private:

	void CreatePSO(AllPipelineSet* allPipelineset_);

};

