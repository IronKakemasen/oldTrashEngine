#include "IndexBuffer.h"
#include "../gpuResourcesBehavior.h"

void IndexBuffer::Create(ID3D12Device* device_, UINT sizeOfBuffer_)
{
	// インデックスバッファ生成
	buffer = CreateBufferResource(device_, sizeOfBuffer_);

	// インデックスバッファビューの作成
	view.BufferLocation = buffer->GetGPUVirtualAddress();
	view.Format = DXGI_FORMAT_R32_UINT;
	view.SizeInBytes = sizeOfBuffer_;

}