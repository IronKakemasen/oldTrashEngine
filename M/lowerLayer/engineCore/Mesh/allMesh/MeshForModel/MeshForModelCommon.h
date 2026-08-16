#pragma once
#include <stdint.h>
#include <wrl.h>
#include <d3d12.h>
#include <memory>
#include <assert.h>
#include <vector>

#include "../../../Buffer/constantBuffer/TransformMatrixBuffer/TransformMatrixBuffer.h"
#include "../../../Buffer/constantBuffer/MaterialBuffer/MaterialBuffer.h"
#include "../../../Buffer/VertexBuffer/VertexBuffer.h"
#include "../../../Buffer/IndexBuffer/IndexBuffer.h"
#include "../../meshStructure/ModelData/ModelData.h"

class AllPipelineSet;

struct MeshForModel
{
	//マテリアルバッファ
	MaterialBuffer materialBuffer;
	//World,WVP
	TransformMatrixBuffer transformMatrixBuffer;
	// 頂点バッファとそのびゅー
	VertexBuffer vertexBuffer;
	// インデックスバッファとそのびゅー
	IndexBuffer indexBuffer;
	Vertex* vertexMap = nullptr;
	uint32_t* indexMap = nullptr;

};

struct MeshAndDataCommon
{
private:

	std::vector < Appearance > appearance;
	std::vector < MeshForModel > meshForModel;
	ModelData modelData;

public:

	virtual void CreateMesh(ID3D12Device* device_, int meshIndex_) = 0;
	void CreatePSO(AllPipelineSet* allPipelineset_);


	void Batch();
	void ResizeMeshSize(int size_)
	{
		meshForModel.resize(size_);
		appearance.resize(size_);
	}

	auto* GetAppearance()
	{
		return &appearance;
	}
	auto* GetAppearance(int index_)
	{
		return &appearance[index_];
	}

	auto* Getter_MeshForModel(int index_)
	{
		return &meshForModel[index_];
	}

	auto* Getter_ModelDataOfResMeshes(int index_)
	{
		return &modelData.resMesh[index_];
	}

	auto* Getter_ModelDataOfResMaterials(int index_)
	{
		return &modelData.resMaterial[index_];
	}

	auto& Getter_ModelData()
	{
		return modelData;
	}

	auto* Getter_MeshForModel()
	{
		return &meshForModel;
	}

};