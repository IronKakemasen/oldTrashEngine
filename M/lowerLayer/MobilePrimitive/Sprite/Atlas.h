#pragma once
#include "../MobileBehavior.h"

struct Atlas
{
	float index;
	float uvCoordinateRate;

	void ChangeIndex(float index_);
	float CalcUvPosX();
	Atlas();
};

