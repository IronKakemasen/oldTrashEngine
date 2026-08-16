#pragma once
#include <stdint.h>
#include <vector>
#include "../../../Buffer/VertexBuffer/VertexBuffer.h"
#include "../../../Buffer/constantBuffer/MatrixBuffer/MatrixBuffer.h"
#include "../../meshStructure/VertexForLine/VertexForLine.h"
#include "../../../Buffer/constantBuffer/MaterialBuffer/MaterialBuffer.h"


class AllPipelineSet;
struct ID3D12Device;

class LineMesh
{
public:

	enum MaxDraw
	{
		kLineMax = 6000 
	};

	uint16_t maxDraw;
	UINT curDrawIndex;
	VertexBufferForLine vertexBufferForLine;
	// マトリクスバッファ
	MatrixBuffer viewProjectionMatrixBuffer;
	// 頂点バッファマップ
	Vector3* vertexMap = nullptr;
	std::vector<MaterialForLineBuffer> materialForLineBuffers;

	void Init(uint16_t maxDraw_ , AllPipelineSet* allPipelineset_);
	void CreatePSO(AllPipelineSet* allPipelineset_);

public:

	LineMesh(AllPipelineSet* allPipelineset_, ID3D12Device* device_);
	void DrawIndexReset();
	void DetectOverDrawing();
	UINT& GetCurrentIndex();
	Vector3* GetVertexMap();
	void SetViewProjectionMatrix(Matrix4* src_);
	void SetMaterial(Vector4* color_,UINT index_);
	D3D12_GPU_VIRTUAL_ADDRESS GetMaterialVirtualPtr(UINT index_);
	D3D12_GPU_VIRTUAL_ADDRESS GetViewProjectionVirtualPtr();
	void CreateMesh(ID3D12Device* device_);

	auto* GetVertexBufferView()
	{
		return &vertexBufferForLine.view;
	}

};

