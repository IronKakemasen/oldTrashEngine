#include "colorBuffer.h"
#include "../../../../DescriptorHeap/rtvDescriptorHeap/rtvDescriptorHeap.h"
#include <assert.h>

void ColorBuffer::SetDXMatrix(float width_, float height_, float left_, float top_)
{
	//クライアント領域と一緒のサイズにして画面全体に表示
	viewport.Width = static_cast<FLOAT>(width);
	viewport.Height = static_cast<FLOAT>(height);
	viewport.TopLeftX = left_;
	viewport.TopLeftY = top_;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	//しざー矩形
	scissorRect.right = static_cast<LONG>(width);
	scissorRect.bottom = static_cast<LONG>(height);
	scissorRect.left = static_cast<LONG>(left_);
	scissorRect.top = static_cast<LONG>(top_);
}

[[nodiscard]] D3D12_RENDER_TARGET_VIEW_DESC ColorBuffer::CreateRTV_Desc(DXGI_FORMAT format_, D3D12_RTV_DIMENSION dimension_,
	UINT planeSlice_, UINT mipSlice_)
{
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};

	//出力結果をSRGBに変換する
	rtvDesc.Format = format_;
	//2Dテクスチャとして書き込む
	rtvDesc.ViewDimension = dimension_;
	rtvDesc.Texture2D.PlaneSlice = planeSlice_;
	rtvDesc.Texture2D.MipSlice = mipSlice_;

	return rtvDesc;
}

void ColorBuffer::CreateRTV(ID3D12Device* device_ , RtvDescriptorHeap* rtvDescHeap_, 
	D3D12_RENDER_TARGET_VIEW_DESC desc_, ID3D12Resource* resource_)
{
	if (cur_index > 0)
	{
		handle.ptr = rtvDescHeap_->GetCPUDescriptorHandle(cur_index);

		device_->CreateRenderTargetView(resource_, &desc_, handle);
	}

	else
	{
		//ディスクリプタの先頭を取得する
		D3D12_CPU_DESCRIPTOR_HANDLE rtvStartHandle = rtvDescHeap_->Getter_Descriptorheap()->GetCPUDescriptorHandleForHeapStart();
		//1つめを作る。１つ目は最初に作る。作る場所をこちらで指定する必要がある
		handle = rtvStartHandle;

		device_->CreateRenderTargetView(resource_, &desc_, handle);
	}

	if (++cur_index > rtvDescHeap_->Getter_MaxDescriptor()) assert(false);


}
