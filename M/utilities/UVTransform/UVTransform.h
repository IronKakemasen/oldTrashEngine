#pragma once
#include "../vectorAndMatrix/vectorAndMatrix.h"

struct UVTransform
{
	Vector2 pos = { 0.0f,0.0f };
	Vector2 scale = { 1.0f,1.0f };
	float rotate = 0.0f;

	UVTransform* parent = nullptr;

	//UV行列の更新
	Matrix4 GetUVMat();
	UVTransform();
};
