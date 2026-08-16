#include "Atlas.h"

void Atlas::ChangeIndex(float index_)
{
	index = index_;
}

float Atlas::CalcUvPosX()
{
	float const numDefault = uvCoordinateRate * 0.5f;

	return uvCoordinateRate * index + numDefault;
}

Atlas::Atlas()
{
	index = 0;
	uvCoordinateRate = 0;
}
