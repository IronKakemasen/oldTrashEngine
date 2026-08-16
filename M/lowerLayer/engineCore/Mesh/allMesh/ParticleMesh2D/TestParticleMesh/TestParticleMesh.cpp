#include "TestParticleMesh.h"
#include "../../../../../MobilePrimitive/MobileQuad/MQuad/MQuad.h"
#include "../../../../Buffer/gpuResources/Creator/SrvCreator/ParticleMeshSrvCreator/ParticleMeshSrvCreator.h"

void TestParticleMesh::CreateMesh(ID3D12Device* device_, ParticleMeshSrvCreator* particleMeshSrvCreator_)
{
	// 頂点データのサイズ
	UINT sizeOfVertexBuffer = static_cast<UINT>(sizeof(Vertex) * vertexCnt);
	// 頂点バッファ生成
	vertexBuffer.Create(device_, sizeOfVertexBuffer);

	// 頂点バッファのマッピング
	MQuad mQuad;
	mQuad.SetVertex(width, height);
	mQuad.SetNormal();
	mQuad.SetTexcoord();

	Vertex vData[4] =
	{
		mQuad.leftBottom,mQuad.leftTop,mQuad.rightBottom,mQuad.rightTop
	};

	HRESULT result = vertexBuffer.buffer->Map(0, nullptr, reinterpret_cast<void**>(&vertexMap));
	assert(SUCCEEDED(result));
	std::memcpy(&vertexMap[0], vData, sizeof(Vertex) * vertexCnt);
	//vertexBuff->Unmap();

	// インデックスデータのサイズ
	UINT sizeOfIndexBuffer = static_cast<UINT>(sizeof(uint32_t) * indexCnt);
	// インデックスバッファ生成
	indexBuffer.Create(device_, sizeOfIndexBuffer);

	// インデックスバッファのマッピング
	uint32_t indices[6] = { 0,1,2,2,1,3};
	result = indexBuffer.buffer->Map(0, nullptr, reinterpret_cast<void**>(&indexMap));
	assert(SUCCEEDED(result));
	std::memcpy(&indexMap[0], indices, sizeof(uint32_t) * indexCnt);
	//indexBuff->Unmap();

	// 行列バッファ生成
	transformMatrixStructuredBuffer.matrix.CreateAndMapping(device_, (UINT)kMaxDraw);

	// マテリアルバッファ作成
	materialBuffer.material.CreateAndMapping(device_);

	//SRVの作成
	srvIndex = particleMeshSrvCreator_->CreateSRVForParticle(kMaxDraw,
		transformMatrixStructuredBuffer.matrix.shaderBuffer);

}

TestParticleMesh::TestParticleMesh(AllPipelineSet* allPipelineset_)
{
	Init(10, 1.0f, 1.0f, allPipelineset_);
}
