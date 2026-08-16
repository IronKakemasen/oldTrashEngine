#pragma once
#include "../../math/vector/vector.h"

struct Rect
{
	float left;
	float top;
	float bottom;
	float right;

	void SetShape(float width_, float height_, Vector3 center_ = {});
	Rect ConvertToWorld(Vector3 center_);
};
