#include "TextureSrCreator.h"

#include "../../../../../../../utilities/convertString/convertString.h"
#include "../../../../../Essential/BarrierControl/BarrierControl.h"
#include "../../../../../Essential/CommandControl/CommandControl.h"
#include "../../../Resource/shaderBuffer/shaderBuffer.h"
#include "../../../Data/ShaderBufferData/ShaderBufferData.h"

#pragma comment(lib, "d3d12.lib")


//============================================================
// TextureファイルからShaderBufferを作成
//============================================================
int TextureSrCreator::CreateShaderBufferFromFile(
	std::string filePath_,
	ShaderBuffer& data)
{
	//============================================================
	// Textureを読み込む
	//============================================================
	auto mipImages = LoadTextureFile(filePath_);

	// TextureのMetadataを取得
	auto metaData = mipImages.GetMetadata();


	//============================================================
	// Texture Resourceを作成
	//============================================================
	CreateTextureResourceFromMetaData(
		metaData,
		&data);


	//============================================================
	// TextureデータをGPUへUpload
	//============================================================
	UploadTextureData(
		mipImages,
		&data);


	//============================================================
	// SRVを作成
	//============================================================
	//
	// ここは以前、
	//
	// CreateSRVDescFromTexture(
	//     metaData.format,
	//     metaData.mipLevels);
	//
	// だった。
	//
	// Cubemap判定をするため、Metadataをそのまま渡す。
	//============================================================
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc =
		CreateSRVDescFromTexture(metaData);


	//============================================================
	// SRVをDescriptorHeapへ作成
	//============================================================
	uint16_t ret_index = CreateSRV(
		srvDesc,
		&data);


	shaderBufferData->gpuHandleContainer.emplace_back(
		data.handleGPU);


#ifdef USE_IMGUI
	return ret_index - 1;
#endif // USE_IMGUI

	return ret_index;
}


//============================================================
// SRVの設定を作成
//
// 通常のTexture2D
// DDS Cubemap
// の両方に対応
//============================================================
D3D12_SHADER_RESOURCE_VIEW_DESC
TextureSrCreator::CreateSRVDescFromTexture(
	DirectX::TexMetadata const& metaData_)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};


	//============================================================
	// 共通設定
	//============================================================

	// TextureのFormat
	srvDesc.Format = metaData_.format;

	// Shader4ComponentMapping
	srvDesc.Shader4ComponentMapping =
		D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;


	//============================================================
	// Cubemap
	//============================================================
	if (metaData_.IsCubemap())
	{
		srvDesc.ViewDimension =
			D3D12_SRV_DIMENSION_TEXTURECUBE;


		// 一番細かいMipから使用
		srvDesc.TextureCube.MostDetailedMip = 0;


		// Metadataに入っているMipMap数を使用
		srvDesc.TextureCube.MipLevels =
			static_cast<UINT>(metaData_.mipLevels);


		// MipMapのLOD Clamp
		srvDesc.TextureCube.ResourceMinLODClamp = 0.0f;
	}
	//============================================================
	// 通常のTexture2D
	//============================================================
	else
	{
		srvDesc.ViewDimension =
			D3D12_SRV_DIMENSION_TEXTURE2D;


		// 一番細かいMipから使用
		srvDesc.Texture2D.MostDetailedMip = 0;


		// Metadataに入っているMipMap数を使用
		srvDesc.Texture2D.MipLevels =
			static_cast<UINT>(metaData_.mipLevels);


		// Plane
		srvDesc.Texture2D.PlaneSlice = 0;


		// MipMapのLOD Clamp
		srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
	}


	return srvDesc;
}


//============================================================
// TextureデータをGPUへUpload
//============================================================
void TextureSrCreator::UploadTextureData(
	DirectX::ScratchImage const& mipImages_,
	ShaderBuffer* shaderBuffer_)
{
	std::vector<D3D12_SUBRESOURCE_DATA> subresource;


	//============================================================
	// ScratchImageからUpload用Subresourceを作成
	//============================================================
	DirectX::PrepareUpload(
		device,
		mipImages_.GetImages(),
		mipImages_.GetImageCount(),
		mipImages_.GetMetadata(),
		subresource);


	//============================================================
	// Upload用Bufferのサイズを取得
	//============================================================
	UINT intermediateSize =
		static_cast<UINT>(
			GetRequiredIntermediateSize(
				shaderBuffer_->resource.Get(),
				0,
				UINT(subresource.size())));


	//============================================================
	// Upload用Resourceを作成
	//============================================================
	shaderBuffer_->intermediateResource =
		CreateBufferResource(
			device,
			intermediateSize);


	//============================================================
	// CPU側のTextureデータをGPU Textureへコピー
	//============================================================
	UpdateSubresources(
		commandControl->Getter_commandList(),
		shaderBuffer_->resource.Get(),
		shaderBuffer_->intermediateResource.Get(),
		0,
		0,
		UINT(subresource.size()),
		subresource.data());


	//============================================================
	// Textureへの転送後は利用できるよう、
	//
	// D3D12_RESOURCE_STATE_COPY_DEST
	//
	// ↓
	//
	// D3D12_RESOURCE_STATE_GENERIC_READ
	//
	// へResourceStateを変更
	//============================================================
	D3D12_RESOURCE_BARRIER barrier =
		BarrierControl::Create(
			shaderBuffer_->resource.Get(),
			D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
			D3D12_RESOURCE_BARRIER_FLAG_NONE,
			D3D12_RESOURCE_STATE_COPY_DEST,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES);


	BarrierControl::Pitch(
		commandControl->Getter_commandList(),
		&barrier);
}


//============================================================
// Textureファイルを読み込む
//
// DDSの場合
//     LoadFromDDSFile()
//
// PNG / JPGの場合
//     LoadFromWICFile()
//
// を使用する。
//============================================================
DirectX::ScratchImage TextureSrCreator::LoadTextureFile(
	std::string const& filePath_)
{
	DirectX::ScratchImage image{};

	std::wstring filePathW =
		ConvertString(filePath_);


	HRESULT hr = S_OK;


	//============================================================
	// DDSかどうかを判定
	//============================================================
	bool isDDS = false;


	if (filePath_.size() >= 4)
	{
		std::string extension =
			filePath_.substr(
				filePath_.size() - 4);


		// 小文字の .dds
		if (extension == ".dds")
		{
			isDDS = true;
		}

		// 大文字の .DDS
		if (extension == ".DDS")
		{
			isDDS = true;
		}
	}


	//============================================================
	// DDS
	//============================================================
	if (isDDS)
	{
		hr = DirectX::LoadFromDDSFile(
			filePathW.c_str(),
			DirectX::DDS_FLAGS_NONE,
			nullptr,
			image);


		assert(SUCCEEDED(hr));


		//========================================================
		// DDSはファイル内のMipMapをそのまま使用
		//
		// Cubemapの場合も、
		// image.GetMetadata() にCubemap情報が入っている。
		//========================================================
		return image;
	}


	//============================================================
	// PNG / JPGなど
	//============================================================
	hr = DirectX::LoadFromWICFile(
		filePathW.c_str(),
		DirectX::WIC_FLAGS_FORCE_SRGB,
		nullptr,
		image);


	assert(SUCCEEDED(hr));


	//============================================================
	// PNG / JPGはMipMapを生成
	//============================================================
	DirectX::ScratchImage mipImages{};


	hr = DirectX::GenerateMipMaps(
		image.GetImages(),
		image.GetImageCount(),
		image.GetMetadata(),
		DirectX::TEX_FILTER_SRGB,
		0,
		mipImages);


	assert(SUCCEEDED(hr));


	//============================================================
	// MipMap付きのTextureを返す
	//============================================================
	return mipImages;
}


//============================================================
// Constructor
//============================================================
TextureSrCreator::TextureSrCreator()
{

}


//============================================================
// Texture Resourceを作成
//============================================================
void TextureSrCreator::CreateTextureResourceFromMetaData(
	DirectX::TexMetadata& metaData_,
	ShaderBuffer* shaderBuffer_)
{
	D3D12_RESOURCE_DESC resourceDesc{};


	//============================================================
	// Textureの設定
	//============================================================

	// Textureの幅
	resourceDesc.Width =
		UINT(metaData_.width);


	// Textureの高さ
	resourceDesc.Height =
		UINT(metaData_.height);


	// Mipmapの数
	resourceDesc.MipLevels =
		UINT16(metaData_.mipLevels);


	// 奥行き or Textureの配列数
	resourceDesc.DepthOrArraySize =
		UINT16(metaData_.arraySize);


	// TextureのFormat
	resourceDesc.Format =
		metaData_.format;


	// サンプリングカウント
	resourceDesc.SampleDesc.Count = 1;


	// Textureの次元数
	resourceDesc.Dimension =
		D3D12_RESOURCE_DIMENSION(
			metaData_.dimension);


	//============================================================
	// Heapの設定
	//============================================================
	D3D12_HEAP_PROPERTIES heapProperties{};

	heapProperties.Type =
		D3D12_HEAP_TYPE_DEFAULT;


	//============================================================
	// Resourceを生成
	//============================================================
	HRESULT hr = device->CreateCommittedResource(
		&heapProperties,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(
			shaderBuffer_->resource.GetAddressOf()));


	assert(SUCCEEDED(hr));
}