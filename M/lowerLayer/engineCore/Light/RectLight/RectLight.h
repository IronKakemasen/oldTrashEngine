#pragma once
#include "../LightBehavior.h"

class RectLight :LightBehavior
{
	RectLightPara para;
	Vector3 lookDir;

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

	inline auto* GetLookDirection()
	{
		return &lookDir;
	}

	RectLight();
};

