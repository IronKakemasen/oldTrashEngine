#pragma once
#include "../LightBehavior.h"

class PointLight :LightBehavior
{
	PointLightPara para;

public:
	virtual void Update() override;
	virtual void Init() override;
	virtual void Reset() override;

	inline auto* Getter_Para()
	{
		return &para;
	}

	inline auto IsActive()
	{
		return para.isActive;
	}

	PointLight();
};

