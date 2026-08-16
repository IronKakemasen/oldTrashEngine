#pragma once
#include "../../../../../utilities/vectorAndMatrix/vectorAndMatrix.h"

struct Vertex
{
	Vector3 position;
	Vector2 texcoord;
	Vector3 normal;
	Vector3 tangent;

	void Init(Vector3 pos_, Vector3 diffVec1_, Vector3 diffVec2_, Vector2 texcoord_);

};
