#include "TextureSrCreator.h"
#include "../../../../../../../utilities/convertString/convertString.h"
#include "../../../../../Essential/BarrierControl/BarrierControl.h"
#include "../../../../../Essential/CommandControl/CommandControl.h"
#include "../../../Resource/shaderBuffer/shaderBuffer.h"
#include "../../../Data/ShaderBufferData/ShaderBufferData.h"
#pragma comment(lib,"d3d12.lib")


int TextureSrCreator::CreateShaderBufferFromFile(std::string filePath_, ShaderBuffer& data)
{
	auto mipImages = LoadTextureFile(filePath_);
	auto metaData = mipImages.GetMetadata();

	CreateTextureResourceFromMetaData(metaData,&data);
	UploadTextureData(mipImages,&data);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = CreateSRVDescFromTexture(metaData.format, metaData.mipLevels);

	uint16_t ret_index = CreateSRV(srvDesc, &data);

	shaderBufferData->gpuHandleContainer.emplace_back(data.handleGPU);


#ifdef USE_IMGUI
	return ret_index - 1;
#endif // USE_IMGUI

	return ret_index;
}


D3D12_SHADER_RESOURCE_VIEW_DESC TextureSrCreator::CreateSRVDescFromTexture(
	DXGI_FORMAT metaDataFormat_, size_t mipLevels_)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metaDataFormat_;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = UINT(mipLevels_);
	//srvDesc.Texture2D.MostDetailedMip = 0;
	//srvDesc.Texture2D.PlaneSlice = 0;
	//srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;

	return srvDesc;
}

void TextureSrCreator::UploadTextureData(DirectX::ScratchImage const& mipImages_, ShaderBuffer* shaderBuffer_)
{
	std::vector<D3D12_SUBRESOURCE_DATA> subresource;

	DirectX::PrepareUpload(device, mipImages_.GetImages(),
		mipImages_.GetImageCount(), mipImages_.GetMetadata(), subresource);


	UINT intermediateSize = static_cast<UINT>(GetRequiredIntermediateSize(shaderBuffer_->resource.Get(), 0, UINT(subresource.size())));

	shaderBuffer_->intermediateResource = CreateBufferResource(device, intermediateSize);

	UpdateSubresources(commandControl->Getter_commandList(), shaderBuffer_->resource.Get(), shaderBuffer_->intermediateResource.Get(), 0, 0,
		UINT(subresource.size()), subresource.data());

	//Textureへの転送後は利用できるよう、D3D2_RESOUTRCE_STATE_COPY_DESTから
	//D3D12_RESOURCE_STATE_GENERIC_READへResourceStateを変更する
	D3D12_RESOURCE_BARRIER barrier = BarrierControl::Create(
		shaderBuffer_->resource.Get(),
		D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
		D3D12_RESOURCE_BARRIER_FLAG_NONE,
		D3D12_RESOURCE_STATE_COPY_DEST,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES);

	BarrierControl::Pitch(commandControl->Getter_commandList(), &barrier);
}


//テクスチャを読み込むための関数
DirectX::ScratchImage TextureSrCreator::LoadTextureFile(std::string const& filePath_)
{
	//テクスチャファイルを読み込んでプログラムで扱えるようにする
	DirectX::ScratchImage image{};
	std::wstring filePathW = ConvertString(filePath_);
	HRESULT hr = DirectX::LoadFromWICFile(filePathW.c_str(),
		DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
	assert(SUCCEEDED(hr));

	//ミップマップの作成
	DirectX::ScratchImage mipImages{};
	hr = DirectX::GenerateMipMaps(
		image.GetImages(),
		image.GetImageCount(),
		image.GetMetadata(),
		DirectX::TEX_FILTER_SRGB, 0,
		mipImages);

	assert(SUCCEEDED(hr));

	//ミップマップ付きのデータを返す
	return mipImages;
}

TextureSrCreator::TextureSrCreator()
{

}

void TextureSrCreator::CreateTextureResourceFromMetaData(DirectX::TexMetadata& metaData_, ShaderBuffer* shaderBuffer_)
{
	D3D12_RESOURCE_DESC resourceDesc{};

	//[ materialDataを元にResourceの設定 ]
	//textureの幅
	resourceDesc.Width = UINT(metaData_.width);
	//textureの高さ
	resourceDesc.Height = UINT(metaData_.height);
	//mipmapの数
	resourceDesc.MipLevels = UINT16(metaData_.mipLevels);
	//奥行orTextureの配列数
	resourceDesc.DepthOrArraySize = UINT16(metaData_.arraySize);
	//Textureのformat
	resourceDesc.Format = metaData_.format;
	//DXGI_FORMAT_B8G8R8A8_UNORM_SRGB
	//サンプリングカウント。１固定
	resourceDesc.SampleDesc.Count = 1;
	//テクスチャの次元数。2
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION(metaData_.dimension);

	//[ 利用するHeapの設定 ]
	//細かい設定を行う
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;

	//[ resourceを生成する ]
	HRESULT hr = device->CreateCommittedResource(
		&heapProperties,					//Heapの設定
		D3D12_HEAP_FLAG_NONE,				//Heapの特殊な設定、特になし
		&resourceDesc,						//Resourceの設定
		D3D12_RESOURCE_STATE_COPY_DEST,		//データ転送される設定
		nullptr,									//Clear最適値。使わないのでnullptr
		IID_PPV_ARGS(shaderBuffer_->resource.GetAddressOf()));				//作成するresourceへのポインタのポインタ

	assert(SUCCEEDED(hr));

}
