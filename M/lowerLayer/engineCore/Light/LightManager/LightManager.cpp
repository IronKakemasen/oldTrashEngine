#include "LightManager.h"
#include "../../Buffer/StructuredBuffer/PointLightStructuredBuffer/PointLightStructuredBuffer.h"
#include "../../Buffer/constantBuffer/DirectionalLightBuffer/DirectionalLightBuffer.h"
#include "../../../../commonVariables.h"

void LightManager::Update()
{	
	int sum = (int)pointLights.size();
	for (int i = 0; i < sum; ++i)
	{
		if (!pointLights[i]->IsActive())
		{
			continue;
		}

		pointLights[i]->Update();
	}

	sum = (int)rectLights.size();
	for (int i = 0; i < sum; ++i)
	{
		if (!rectLights[i]->IsActive())
		{
			continue;
		}

		rectLights[i]->Update();
	}

	directionalLight->Update();

	Batch();

}


LightManager::LightManager()
{
	pointLightID = 0;
	rectLightID = 0;
}

void LightManager::Init(ExclusiveDraw* exclusiveDraw_, ID3D12Device* device_,
	StructuredBufferSrvCreator* srvCreator_)
{
	static bool onlyOnce = 1;
	if (onlyOnce)
	{
		lightCreator.Init(exclusiveDraw_, device_, srvCreator_);
		Fetch();

		int sum = (int)pointLights.size();
		for (int i = 0; i < sum; ++i)
		{
			pointLights[i]->Init();
		}

		directionalLight->Init();

		onlyOnce = 0;
	}
}

void LightManager::Fetch()
{
	directionalLight = lightCreator.Getter_DirLight();
	dirLightBuffer = lightCreator.Getter_DirLightBuffer();

	auto* pLightContainer = lightCreator.Getter_PointLightContainer();
	pointLightStructuredBuffer = lightCreator.Getter_PointLightStructuredBuffer();

	auto* rLightContainer = lightCreator.Getter_RectLightContainer();
	rectLightStructuredBuffer = lightCreator.Getter_RectLightStructuredBuffer();

	for (auto itr = pLightContainer->begin(); itr != pLightContainer->end(); ++itr)
	{
		pointLights.emplace_back((*itr).get());
	}

	for (auto itr = rLightContainer->begin(); itr != rLightContainer->end(); ++itr)
	{
		rectLights.emplace_back((*itr).get());
	}

}

DirectionalLight* LightManager::ExportDirectionalLight()
{
	return directionalLight;
}

PointLight* LightManager::ExportPointLight()
{
	auto* pLightContainer = lightCreator.Getter_PointLightContainer();
	assert(pointLightID < LightCreator::Max::kPLightMax);

	return pointLights[pointLightID++];
}

RectLight* LightManager::ExportRectLight()
{
	auto* rectLightContainer = lightCreator.Getter_RectLightContainer();
	assert(rectLightID < LightCreator::Max::kRLightmax);

	return rectLights[rectLightID++];
}


void LightManager::Batch()
{
	//PointLights
	for (int i = 0; i < LightCreator::kPLightMax; ++i)
	{
		auto *para = pointLights[i]->Getter_Para();
		pointLightStructuredBuffer->pLight.buffMap[i].isActive = para->isActive;

		if (!para->isActive)
		{
			continue;
		}

		Vector3 color_ = para->color * CommonV::inv_255;
		float invsqrtRadius = 1.0f / (para->invSqrRadius * para->invSqrRadius);

		pointLightStructuredBuffer->pLight.buffMap[i].color = color_;
		pointLightStructuredBuffer->pLight.buffMap[i].intensity = para->intensity;
		pointLightStructuredBuffer->pLight.buffMap[i].pos = para->pos;
		pointLightStructuredBuffer->pLight.buffMap[i].invSqrRadius = invsqrtRadius;
	}

	//RectLights
	for (int i = 0; i < LightCreator::kRLightmax; ++i)
	{
		auto* para = rectLights[i]->Getter_Para();
		rectLightStructuredBuffer->rLight.buffMap[i].isActive = para->isActive;

		if (!para->isActive)
		{
			continue;
		}

		rectLights[i]->Update();

		Vector3 color_ = para->color * CommonV::inv_255;
		rectLightStructuredBuffer->rLight.buffMap[i].color = color_;
		rectLightStructuredBuffer->rLight.buffMap[i].intensity = para->intensity;
		rectLightStructuredBuffer->rLight.buffMap[i].pos = para->pos;
		rectLightStructuredBuffer->rLight.buffMap[i].attenuationRadius = para->attenuationRadius;
		rectLightStructuredBuffer->rLight.buffMap[i].width = para->width;
		rectLightStructuredBuffer->rLight.buffMap[i].height = para->height;
		rectLightStructuredBuffer->rLight.buffMap[i].axisX = para->axisX;
		rectLightStructuredBuffer->rLight.buffMap[i].axisY = para->axisY;
		rectLightStructuredBuffer->rLight.buffMap[i].halfSize =
		{ para->width * 0.5f, para->height * 0.5f };
	}


	//DirectionalLight
	auto* para = directionalLight->Getter_Para();
	Vector3 color_ = para->color * CommonV::inv_255;

	dirLightBuffer->dirLight.buffMap->color = color_;
	dirLightBuffer->dirLight.buffMap->intensity = para->intensity;
	dirLightBuffer->dirLight.buffMap->pos = para->pos;
	dirLightBuffer->dirLight.buffMap->isActive = para->isActive;
}
