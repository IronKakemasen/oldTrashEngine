#include "Transform.h"
#include "../Easing/EasingFunctions.h"

Matrix4 Transform::GetWorldMatrix()
{
	RotateUpdate();

	Matrix4 ret_world = Get_SQrTMat3D(scale, quaternion.q, pos);
	if (parent)
	{
		Matrix4 parentMat = parent->GetWorldMatrix();
		ret_world = ret_world.Multiply(parentMat);
	}

	return ret_world;
}

void Transform::BeChildren(Transform* parent_)
{
	parent = parent_;
}

Vector3 Transform::GetWorldPos()
{
	Matrix4 wMat = GetWorldMatrix();

	return
	{
		wMat.m[3][0],
		wMat.m[3][1],
		wMat.m[3][2],
	};
}

Transform::Transform()
{
	Init();
}

void Transform::Init()
{
	rotation = { 0,0,0 };
	scale = { 1.0f,1.0f,1.0f };
	lookDir = V_Vector::kBeyond;
	interpolationCoe = 1.0f;


}

void Transform::RotateUpdate()
{
	Vector4 composite;

	Vector4 rightQua = GetQuaternion(V_Vector::kRight, rotation.x);
	Vector4 upQua = GetQuaternion(V_Vector::kUp, rotation.y);
	Vector4 beyondQua = GetQuaternion(V_Vector::kBeyond, rotation.z);

	composite = GetCompositeQuaternion(upQua, rightQua);
	composite = GetCompositeQuaternion(composite, beyondQua);

	//ターゲット方向のクォータニオン(SLERP)
	lookDir = lookDir.GetNormalized();
	//Benri::Adjust(lookDir.z, -0.00001f, 0.00001f, 0.00001f);

	Quaternion lookAtQua =
		Easing::SlerpQuaternion(quaternion, lookDir, interpolationCoe);
	quaternion.q = GetCompositeQuaternion(composite, lookAtQua.q);

}