#include "LightCreator.h"
#include "../../Render/ExclusiveDraw/ExclusiveDraw.h"
#include "../../Buffer/gpuResources/Creator/SrvCreator/StructuredBufferSrvCreator/StructuredBufferSrvCreator.h"

LightCreator::LightCreator()
{

}

void LightCreator::CreatePointLight(ExclusiveDraw* exclusiveDraw_, ID3D12Device* device_, 
	StructuredBufferSrvCreator* srvCreator_)
{
	pLightStructuredBuffer.reset(new PointLightStructuredBuffer);
	pLightStructuredBuffer->pLight.CreateAndMapping(device_, Max::kPLightMax);

	for (int i = 0; i < kPLightMax; ++i)
	{
		pointLightContainer.emplace_back(std::make_unique<PointLight>());
	}

	//SRVの作成
	pointLightSrvIndex = srvCreator_->CreateSRVForStructuredBuffer(kPLightMax, sizeof(PointLightPara),
		pLightStructuredBuffer->pLight.shaderBuffer);

	exclusiveDraw_->Setter_PLightSrvIndex(&pointLightSrvIndex);

}

void LightCreator::CreateRectLight(ExclusiveDraw* exclusiveDraw_, ID3D12Device* device_,
	StructuredBufferSrvCreator* srvCreator_)
{
	rectLightStructuredBuffer.reset(new RectLightStructuredBuffer);
	rectLightStructuredBuffer->rLight.CreateAndMapping(device_, kRLightmax);

	for (int i = 0; i < kRLightmax; ++i)
	{
		rectLightContainer.emplace_back(std::make_unique<RectLight>());
	}

	//SRVの作成
	rectLightSrvIndex = srvCreator_->CreateSRVForStructuredBuffer(kRLightmax, sizeof(RectLightPara),
		rectLightStructuredBuffer->rLight.shaderBuffer);

	exclusiveDraw_->Setter_RLightSrvIndex(&rectLightSrvIndex);

}

void LightCreator::CreateDirectionalLight(ExclusiveDraw* exclusiveDraw_, ID3D12Device* device_)
{

	dirLightSet.first.reset(new DirectionalLightBuffer);
	dirLightSet.second.reset(new DirectionalLight());

	dirLightSet.first->dirLight.CreateAndMapping(device_);
	exclusiveDraw_->Setter_DirectionalLightBuffer(dirLightSet.first.get());
}

void LightCreator::Init(ExclusiveDraw* exclusiveDraw_, ID3D12Device* device_,
	StructuredBufferSrvCreator* srvCreator_)
{
	CreateRectLight(exclusiveDraw_, device_, srvCreator_);
	CreatePointLight(exclusiveDraw_,device_,srvCreator_);
	CreateDirectionalLight(exclusiveDraw_, device_);
}
