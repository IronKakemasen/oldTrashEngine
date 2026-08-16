#include "textureDataManager.h"
#include "../DescriptorHeap/srvDescriptorHeap/srvDescriptorHeap.h"
#include "../Buffer/gpuResources/Creator/SrvCreator/TextureSrCreator/TextureSrCreator.h"
#include "../Buffer/gpuResources/Resource/shaderBuffer/shaderBuffer.h"
#include "../Buffer/gpuResources/Creator/SrvCreator/PostEffectSrCreator/PostEffectSrCreator.h"
#include "../DescriptorHeap/rtvDescriptorHeap/rtvDescriptorHeap.h"

void TextureDataManager::Init(TextureSrCreator* textureSrvCreator_, PostEffectSrCreator* postEffectSrCreator_, 
	RtvDescriptorHeap* rtvDescriptorHeap_, ID3D12Device* device_)
{
	textureSrvCreator = textureSrvCreator_;
	postEffectSrCreator = postEffectSrCreator_;
	rtvDescriptorHeap = rtvDescriptorHeap_;
	device = device_;
}

int TextureDataManager::CreateTextureFromFile( std::string filePath_)
{
	auto& dst = data.emplace_back();
	return textureSrvCreator->CreateShaderBufferFromFile(filePath_, dst);
}

PostEffectBuffer* TextureDataManager::CreatePostEffectBuffer(float width_ ,float height_)
{
	auto& buffer = 
		postEffectBufferContaier.emplace_back(postEffectSrCreator->Create(width_ , height_));

	D3D12_RENDER_TARGET_VIEW_DESC iranai {};
	buffer->CreateRTV(device, rtvDescriptorHeap, iranai);

	//D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	//rtvDesc = ColorBuffer::CreateRTV_Desc();
	//buffer->CreateRTV(device, rtvDescriptorHeap, rtvDesc);


	return buffer.get();
}