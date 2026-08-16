#include "PostEffectSrCreator.h"
#include <assert.h>
#include "../../commonVariables.h"
#include "../../../Data/ShaderBufferData/ShaderBufferData.h"
#include "../../../../PostEffectBuffer/PostEffectBuffer.h"
#include "../DescriptorHeap/dsvDescriptorHeap/dsvDescriptorHeap.h"

PostEffectSrCreator::PostEffectSrCreator()
{

}

std::unique_ptr<PostEffectBuffer> PostEffectSrCreator::Create(float width_ , float height_)
{
	std::unique_ptr<PostEffectBuffer> buffer = std::make_unique<PostEffectBuffer>();
	buffer->GetColorBuffer()->width = width_;
	buffer->GetColorBuffer()->height = height_;
	CreteResource(buffer.get());
	CreateSRVForPostEffect(buffer.get());
	buffer->CreateDepthBuffer(device, dsvDescriptorHeap);

	if (commonFormat == DXGI_FORMAT_R8G8B8A8_UNORM_SRGB)
	{
		buffer->atoduke = 0;
	}
	else
	{
		buffer->atoduke = 1;
	}

	return std::move(buffer);
}

D3D12_SHADER_RESOURCE_VIEW_DESC PostEffectSrCreator::CreateSrvDesc()
{
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};

	srvDesc.Format = commonFormat;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = UINT(1);
	return srvDesc;
}

void PostEffectSrCreator::CreateSRVForPostEffect(PostEffectBuffer* data_)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC desc = CreateSrvDesc();
	uint16_t srvIndex = CreateSRV(desc, data_->GetShaderBuffer());

	shaderBufferData->gpuHandleContainer.emplace_back(data_->GetShaderBuffer()->handleGPU);

#ifdef USE_IMGUI
	srvIndex--;
#endif // USE_IMGUI

	*data_->GetSrvHandle() = srvIndex;
}

void PostEffectSrCreator::CreteResource(PostEffectBuffer* data_)
{
	D3D12_RESOURCE_DESC resourceDesc{};

	//[ materialDataを元にResourceの設定 ]
	//textureの幅
	resourceDesc.Width = UINT(data_->GetColorBuffer()->width);
	//textureの高さ
	resourceDesc.Height = UINT(data_->GetColorBuffer()->height);
	//mipmapの数
	resourceDesc.MipLevels = 1;
	//奥行orTextureの配列数
	resourceDesc.DepthOrArraySize = 1;
	//Textureのformat
	resourceDesc.Format = commonFormat;
	//サンプリングカウント。１固定
	resourceDesc.SampleDesc.Count = 1;
	//テクスチャの次元数。2
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

	//[ 利用するHeapの設定 ]
	//細かい設定を行う
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;

	/*DXGI_FORMAT_D32_FLOAT*/
	/*DXGI_FORMAT_R32G32B32A32_FLOAT*/
	/*DXGI_FORMAT_R8G8B8A8_UNORM_SRGB*/
	
	// Clear 値を正しく渡す（CreateRenderTargetView と同じフォーマット）
	D3D12_CLEAR_VALUE clearValue{};
	clearValue.Format = commonFormat;
	clearValue.Color[0] = 0.0f;
	clearValue.Color[1] = 0.0f;
	clearValue.Color[2] = 0.0f;
	clearValue.Color[3] = 0.0f;

	HRESULT hr = device->CreateCommittedResource(
		&heapProperties,					//Heapの設定
		D3D12_HEAP_FLAG_NONE,				//Heapの特殊な設定、特になし
		&resourceDesc,						//Resourceの設定
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		&clearValue,							//Clear最適値。使わないのでnullptr
		IID_PPV_ARGS(data_->GetShaderBuffer()->Getter_ResourcePP()));			//作成するresourceへのポインタのポインタ

	assert(SUCCEEDED(hr));

}