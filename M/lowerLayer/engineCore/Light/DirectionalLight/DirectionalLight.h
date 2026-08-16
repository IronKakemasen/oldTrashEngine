#pragma once
#include "../LightBehavior.h"


class DirectionalLight:LightBehavior
{

	LightBasePara para;
public:
	virtual void Update() override;
	virtual void Init() override;
	virtual void Reset() override;
	
	inline auto* Getter_Para()
	{
		return &para;
	}

	DirectionalLight();
};

