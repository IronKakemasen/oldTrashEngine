#pragma once
#include "../SrvCreatorBehavior.h"

#include "../../../../../../../../external/DirectXTex/d3dx12.h"
#include "../../../../../../../../external/DirectXTex/DirectXTex.h"


class TextureSrCreator:public SrvCreatorBehavior
{
	void CreateTextureResourceFromMetaData(DirectX::TexMetadata& metaData_, ShaderBuffer* shaderBuffer_);

	void UploadTextureData(DirectX::ScratchImage const& mipImages_, ShaderBuffer* shaderBuffer_);

	D3D12_SHADER_RESOURCE_VIEW_DESC CreateSRVDescFromTexture(DXGI_FORMAT metaDataFormat_, size_t mipLevels_);

	//テクスチャを読み込むための関数
	DirectX::ScratchImage LoadTextureFile(std::string const& filePath_);

public:
	int CreateShaderBufferFromFile(std::string filePath_, ShaderBuffer& data);
	TextureSrCreator();

};

