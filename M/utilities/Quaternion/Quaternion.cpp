#include "Quaternion.h"
#include "../ConvertToRadian/ConvertToRadian.h"
#include <math.h>

Matrix4 Quaternion::GetRotateMat()
{
	float a00 = (q.x * q.x) - (q.y * q.y) - (q.z * q.z) + (q.w * q.w);
	float a01 = (q.x * q.y - q.z * q.w) * 2.0f;
	float a02 = (q.x * q.z + q.y * q.w) * 2.0f;

	float a10 = (q.x * q.y + q.z * q.w) * 2.0f;
	float a11 = -(q.x * q.x) + (q.y * q.y) - (q.z * q.z) + (q.w * q.w);
	float a12 = (q.y * q.z - q.x * q.w) * 2.0f;

	float a20 = (q.x * q.z - q.y * q.w) * 2.0f;
	float a21 = (q.y * q.z + q.x * q.w) * 2.0f;
	float a22 = -(q.x * q.x) - (q.y * q.y) + (q.z * q.z) + (q.w * q.w);
	return Matrix4
	{
		a00, a10, a20, 0.0f,
		a01, a11, a21, 0.0f,
		a02, a12, a22, 0.0f,
		0.0f,0.0f,0.0f,1.0f,
	};
}

Vector4 Quaternion::CreateQuaternion(Vector3 axis_, float theta_)
{
	float radian = GetRadian(theta_);
	Vector3 buff = axis_.GetNormalized();
	float halfRad = radian * CommonV::kHalf;
	float sinNum = sinf(halfRad);

	Vector4 quaternion = { buff.x * sinNum,buff.y * sinNum
		,buff.z * sinNum ,cosf(halfRad) };

	return quaternion;
}

Vector4 Quaternion::CreateQuaternion(Vector3 axis_, float radian_, bool rad )
{
	Vector3 buff = axis_.GetNormalized();
	float halfRad = radian_ * CommonV::kHalf;
	float sinNum = sinf(halfRad);

	Vector4 quaternion = { buff.x * sinNum,buff.y * sinNum
		,buff.z * sinNum ,cosf(halfRad) };

	return quaternion;
}


Vector4 Quaternion::CreateQuaternion(Vector3 lookDir_)
{
	Vector3 z = lookDir_;
	Vector3 ver = { 0.0f, 1.0f, 0.0f };
	Vector3 x = (ver.GetCross(z)).GetNormalized();
	Vector3 y = (z.GetCross(x)).GetNormalized();

	Matrix4 m =
	{
		x.x,y.x,z.x,0.0f,
		x.y,y.y,z.y,0.0f,
		x.z,y.z,z.z,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};


	return ConvertToQuaternion(m);
}

Vector4 Quaternion::ConvertToQuaternion(Matrix4 m_)
{
	float elem[4];
	elem[0] = m_.m[0][0] - m_.m[1][1] - m_.m[2][2] + 1.0f;
	elem[1] = -m_.m[0][0] + m_.m[1][1] - m_.m[2][2] + 1.0f;
	elem[2] = -m_.m[0][0] - m_.m[1][1] + m_.m[2][2] + 1.0f;
	elem[3] = m_.m[0][0] + m_.m[1][1] + m_.m[2][2] + 1.0f;

	int biggestIdx = 0;

	for (int i = 0; i < 4; i++)
	{
		if (elem[i] > elem[biggestIdx])
		{
			biggestIdx = i;
		}
	}

	if (elem[biggestIdx] < 0)
	{
		return Vector4();
	}

	float q[4];
	float v = sqrtf(elem[biggestIdx]) * 0.5f;
	q[biggestIdx] = v;
	float mult = 0.25f / v;

	switch (biggestIdx)
	{
	case 0:
		q[1] = (m_.m[1][0] + m_.m[0][1]) * mult;
		q[2] = (m_.m[0][2] + m_.m[2][0]) * mult;
		q[3] = (m_.m[2][1] - m_.m[1][2]) * mult;
		break;
	case 1:
		q[0] = (m_.m[1][0] + m_.m[0][1]) * mult;
		q[2] = (m_.m[2][1] + m_.m[1][2]) * mult;
		q[3] = (m_.m[0][2] - m_.m[2][0]) * mult;
		break;
	case 2:
		q[0] = (m_.m[0][2] + m_.m[2][0]) * mult;
		q[1] = (m_.m[2][1] + m_.m[1][2]) * mult;
		q[3] = (m_.m[1][0] - m_.m[0][1]) * mult;
		break;
	case 3:
		q[0] = (m_.m[2][1] - m_.m[1][2]) * mult;
		q[1] = (m_.m[0][2] - m_.m[2][0]) * mult;
		q[2] = (m_.m[1][0] - m_.m[0][1]) * mult;
		break;
	}

	return Vector4 {q[0], q[1], q[2], q[3]};
}

float Quaternion::GetDot(Quaternion other_)
{
	Vector4 tmp = GetNormalized();
	Vector4 tmp2 = other_.GetNormalized();

	return
		tmp.x * tmp2.x +
		tmp.y * tmp2.y +
		tmp.z * tmp2.z +
		tmp.w * tmp2.w;
}

Vector4 Quaternion::GetNormalized()
{
	Vector4 tmp = q;

	float mag = sqrtf(q .w * q.w + q.x * q.x + q.y * q.y + q.z * q.z);
	if (mag > 0.0) 
	{
		float invMag = 1.0f / mag;
		tmp.w *= invMag;
		tmp.x *= invMag;
		tmp.y *= invMag;
		tmp.z *= invMag;
	}

	return tmp;
}

