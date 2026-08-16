#include "ModelSimple.h"

void ModelSimple::CreateMesh(ID3D12Device* device_,int meshIndex_)
{
	auto* resMeshes = Getter_ModelDataOfResMeshes(meshIndex_);
	auto* resMaterials = Getter_ModelDataOfResMaterials(meshIndex_);
	auto* meshForModel = Getter_MeshForModel(meshIndex_);

	// 頂点データのサイズ
	UINT sizeOfVertexBuffer =
		static_cast<UINT>(sizeof(Vertex) * resMeshes->vertices.size());
	// 頂点バッファ生成
	meshForModel->vertexBuffer.Create(device_, sizeOfVertexBuffer);
	//頂点バッファのマッピング
	HRESULT result = meshForModel->vertexBuffer.buffer->Map(0, nullptr, reinterpret_cast<void**>(&meshForModel->vertexMap));
	assert(SUCCEEDED(result));
	//頂点データの入力
	std::memcpy(meshForModel->vertexMap, resMeshes->vertices.data(),
		sizeof(Vertex) * resMeshes->vertices.size());
	meshForModel->vertexBuffer.buffer->Unmap(0, nullptr);

	// インデックスデータのサイズ
	UINT sizeOfIndexBuffer = static_cast<UINT>(sizeof(uint32_t) * resMeshes->indices.size());
	// インデックスバッファ生成
	meshForModel->indexBuffer.Create(device_, sizeOfIndexBuffer);
	//インデックスバッファのマッピング
	result = meshForModel->indexBuffer.buffer->Map(0, nullptr, reinterpret_cast<void**>(&meshForModel->indexMap));
	assert(SUCCEEDED(result));

	//インデックスデータの入力
	std::memcpy(meshForModel->indexMap, resMeshes->indices.data(),
		sizeof(uint32_t) * resMeshes->indices.size());
	meshForModel->indexBuffer.buffer->Unmap(0, nullptr);

	// ワールド行列バッファ生成
	meshForModel->transformMatrixBuffer.matrix.CreateAndMapping(device_);
	// マテリアルバッファ作成1
	meshForModel->materialBuffer.material.CreateAndMapping(device_);

	//ストラクチャーバッファを作成



}

ModelSimple::ModelSimple(AllPipelineSet* allPipelineset_)
{
	CreatePSO(allPipelineset_);
}

ModelSimple::ModelSimple()
{

}
