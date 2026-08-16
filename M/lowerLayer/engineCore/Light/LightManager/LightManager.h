#pragma once
#include "../LightCreator/LightCreator.h"

class LightManager
{
	std::vector<PointLight*> pointLights;
	PointLightStructuredBuffer* pointLightStructuredBuffer = nullptr;
	DirectionalLightBuffer* dirLightBuffer = nullptr;
	DirectionalLight* directionalLight = nullptr;
	std::vector<RectLight*> rectLights;
	RectLightStructuredBuffer* rectLightStructuredBuffer = nullptr;

	LightCreator lightCreator;
	int pointLightID;
	int rectLightID;

	void Fetch();
	void Batch();
public:

	DirectionalLight* ExportDirectionalLight();
	PointLight* ExportPointLight();
	RectLight* ExportRectLight();

	LightManager();

	void Update();
	void Init(ExclusiveDraw* exclusiveDraw_, ID3D12Device* device_,
		StructuredBufferSrvCreator* srvCreator_);
	
	inline auto* Getter_LightCreator()
	{
		return &lightCreator;
	}
};

