#pragma once
#include "../../../../../utilities/vectorAndMatrix/vectorAndMatrix.h"

struct Material
{
	Vector3 diffuse;
	float roughness = 0.0f;
	Vector3 specular;
	float metalic = 0.0f;
	Vector4 albedoColor;
	Matrix4 uvTransform;
	float shininess = 0.0f;

};



