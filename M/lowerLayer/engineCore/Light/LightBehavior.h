#pragma once
#include "../../math/vector/vector.h"

struct LightBasePara
{
	Vector3 pos;
	float intensity;
	Vector3 color;
	int isActive;

	LightBasePara();
};

struct PointLightPara
{
	Vector3 pos;
	float intensity;
	Vector3 color;
	float invSqrRadius;
	int isActive;

	PointLightPara();
};

struct RectLightPara
{
	Vector3 pos;
	float intensity;
	Vector3 color;
	float attenuationRadius;
	Vector3 axisX;
	float width;
	Vector3 axisY;
	float height;
	Vector2 halfSize;
	int isActive;

	RectLightPara();
};


struct LightBehavior 
{
	virtual void Update() = 0;
	virtual void Init() = 0;
	virtual void Reset() = 0;
};

