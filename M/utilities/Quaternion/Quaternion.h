#pragma once
#include "../vectorAndMatrix/vectorAndMatrix.h"

struct Quaternion
{
	Vector4 q;

	Matrix4 GetRotateMat();
	Vector4 GetNormalized();
	float GetDot(Quaternion other_);

	static Vector4 CreateQuaternion(Vector3 lookDir_);
	static Vector4 CreateQuaternion(Vector3 axis_, float theta_);
	static Vector4 CreateQuaternion(Vector3 axis_, float radian_,bool rad = true);

	static Vector4 ConvertToQuaternion(Matrix4 m_);
};

