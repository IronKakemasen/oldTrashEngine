#pragma once
#include <stdint.h>
#include <wrl.h>
#include <d3d12.h>
#include <memory>
#include <assert.h>
#include <vector>
#include "../../meshStructure/Vertex/Vertex.h"
#include "../../../Buffer/constantBuffer/MaterialBuffer/MaterialBuffer.h"
#include "../../../Buffer/StructuredBuffer/TransformMatrixStructuredBuffer/TransformMatrixStructuredBuffer.h"
#include "../../../Buffer/VertexBuffer/VertexBuffer.h"
#include "../../../Buffer/IndexBuffer/IndexBuffer.h"

class AllPipelineSet;
class ParticleMeshSrvCreator;

struct ParticleMesh2DCommon
{
	//マテリアルバッファ
	MaterialBuffer materialBuffer;
	//WVPワールドマトリクスバッファ
	TransformMatrixStructuredBuffer transformMatrixStructuredBuffer;

	uint16_t kMaxDraw = 0;
	UINT vertexCnt = 0;
	UINT indexCnt = 0;
	float width;
	float height;
	//srvのインデックス
	uint16_t srvIndex;

protected:

	// 頂点バッファとそのびゅー
	VertexBuffer vertexBuffer;
	// インデックスバッファとそのびゅー
	IndexBuffer indexBuffer;

	// 頂点バッファマップ
	Vertex* vertexMap = nullptr;
	// インデックスバッファマップ
	uint32_t* indexMap = nullptr;

	virtual void CreateMesh(ID3D12Device* device_, ParticleMeshSrvCreator* ParticleMeshSrvCreator_) = 0;
	void Init(uint16_t kMaxDraw_, float width_, float height_, AllPipelineSet* allPipelineset_);

public:

	ParticleMesh2DCommon();
	void DetectOverDrawing(int sum_draw_);

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
