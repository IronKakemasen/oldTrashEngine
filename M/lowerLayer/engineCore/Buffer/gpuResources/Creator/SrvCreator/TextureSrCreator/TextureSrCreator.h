#pragma once

#include "../SrvCreatorBehavior.h"

#include "../../../../../../../../external/DirectXTex/d3dx12.h"
#include "../../../../../../../../external/DirectXTex/DirectXTex.h"


class TextureSrCreator : public SrvCreatorBehavior
{
private:

	//============================================================
	// Texture Resourceを作成
	//============================================================
	void CreateTextureResourceFromMetaData(
		DirectX::TexMetadata& metaData_,
		ShaderBuffer* shaderBuffer_);


	//============================================================
	// TextureデータをGPUへUpload
	//============================================================
	void UploadTextureData(
		DirectX::ScratchImage const& mipImages_,
		ShaderBuffer* shaderBuffer_);


	//============================================================
	// SRVの設定を作成
	//
	// 通常Texture2D
	// DDS Cubemap
	// の両方に対応
	//============================================================
	D3D12_SHADER_RESOURCE_VIEW_DESC CreateSRVDescFromTexture(
		DirectX::TexMetadata const& metaData_);


	//============================================================
	// テクスチャファイルを読み込む
	//
	// PNG / JPG → WIC
	// DDS      → DirectXTex DDS
	//============================================================
	DirectX::ScratchImage LoadTextureFile(
		std::string const& filePath_);


public:

	int CreateShaderBufferFromFile(
		std::string filePath_,
		ShaderBuffer& data);

	TextureSrCreator();

};