#include "depthBuffer.h"
#include "../../../../DescriptorHeap/dsvDescriptorHeap/dsvDescriptorHeap.h"
#include <assert.h>

[[nodiscard]] D3D12_DEPTH_STENCIL_VIEW_DESC DepthBuffer::CreateDepthStencilViewDesc(
	DXGI_FORMAT format_ ,
	D3D12_DSV_DIMENSION dimension_ ,
	UINT mipSlice_ ,
	D3D12_DSV_FLAGS flags_ 
)
{
	D3D12_DEPTH_STENCIL_VIEW_DESC ret_dsvDesc{};

	//format。基本的にはリソースに合わせる
	ret_dsvDesc.Format = format_;
	//2DTexture
	ret_dsvDesc.ViewDimension = dimension_;
	ret_dsvDesc.Texture2D.MipSlice = mipSlice_;
	ret_dsvDesc.Flags = flags_;

	return ret_dsvDesc;
}

void DepthBuffer::CreateDSV(ID3D12Device* device_, DsvDescriptorHeap* dsvDescHeap_, D3D12_DEPTH_STENCIL_VIEW_DESC desc_)
{
	if (cur_index > 0)
	{
		handle.ptr = dsvDescHeap_->GetCPUDescriptorHandle(cur_index);

		device_->CreateDepthStencilView(resource.Get(), &desc_, handle);
	}

	else
	{
		//ディスクリプタの先頭を取得する
		D3D12_CPU_DESCRIPTOR_HANDLE dsvStartHandle = dsvDescHeap_->Getter_Descriptorheap()->GetCPUDescriptorHandleForHeapStart();
		//1つめを作る。１つ目は最初に作る。作る場所をこちらで指定する必要がある
		handle = dsvStartHandle;

		device_->CreateDepthStencilView(resource.Get(), &desc_, handle);
	}

	cur_index++;

	if (cur_index > dsvDescHeap_->Getter_MaxDescriptor()) assert(false);
}


void DepthBuffer::CreateDepthStencilTextureResource(ID3D12Device* device_, int32_t width_, int32_t height_)
{
	//生成するResourceの設定
	//テクスチャのサイズ
	D3D12_RESOURCE_DESC resourceDesc{};

	resourceDesc.Width = width_;
	resourceDesc.Height = height_;
	resourceDesc.Alignment = 0;
	//mipmapの数
	resourceDesc.MipLevels = 1;
	//奥行or配列Textureの配列数
	resourceDesc.DepthOrArraySize = 1;
	//depthstencilとして利用可能なフォーマット
	//resourceDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	resourceDesc.Format = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
	
	//サンプリングカウント1固定
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	//DepthStencilとして使う通知
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;

	//利用するHeapの設定
	//VRAM上に作る
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;
	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProperties.CreationNodeMask = 1;
	heapProperties.VisibleNodeMask = 1;

	//深度値クリア設定
	//最大値（1.0）でクリア
	D3D12_CLEAR_VALUE depthClearValue{};
	//depthClearValue.DepthStencil.Depth = 1.0f;
	depthClearValue.DepthStencil.Depth = 0.0f;

	//フォーマット。resourceと合わせる
	depthClearValue.Format = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;

	//Resourceの生成
	HRESULT hr = device_->CreateCommittedResource
	(
		&heapProperties,					//Heapの設定
		D3D12_HEAP_FLAG_NONE,				//Heapの特殊な設定：特になし
		&resourceDesc,						//resourceの設定
		D3D12_RESOURCE_STATE_DEPTH_WRITE,	//深度値を書き込む状態にしておく
		&depthClearValue,					//Clear最適値
		IID_PPV_ARGS(&resource)				//作成するリソースポインタへのポインタ
	);

	assert(SUCCEEDED(hr));

}
