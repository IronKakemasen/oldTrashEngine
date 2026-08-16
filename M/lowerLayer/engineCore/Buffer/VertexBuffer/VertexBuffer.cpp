#include "VertexBuffer.h"
#include "../gpuResourcesBehavior.h"
#include "../../Mesh/meshStructure/Vertex/Vertex.h"


void VertexBuffer::Create(ID3D12Device* device_, UINT sizeOfBuffer_)
{
	// 頂点バッファ生成
	buffer = CreateBufferResource(device_, sizeOfBuffer_);

	// 頂点バッファビューの作成
	view.BufferLocation = buffer->GetGPUVirtualAddress();
	view.SizeInBytes = sizeOfBuffer_;
	view.StrideInBytes = sizeof(Vertex);

}

void VertexBufferForLine::Create(ID3D12Device* device_, UINT sizeOfBuffer_)
{
	// 頂点バッファ生成
	buffer = CreateBufferResource(device_, sizeOfBuffer_);

	// 頂点バッファビューの作成
	view.BufferLocation = buffer->GetGPUVirtualAddress();
	view.SizeInBytes = sizeOfBuffer_;
	view.StrideInBytes = sizeof(Vector3);

}

