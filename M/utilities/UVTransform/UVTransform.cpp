#include "UVTransform.h"

Matrix4 UVTransform::GetUVMat()
{
	Matrix4 ret_uvMat;

	if (parent)
	{
		ret_uvMat = Get_SRTMat3D({ scale.x, scale.y ,0.0f }, { 0,0,rotate }, { pos.x,pos.y,0.0f });
		Matrix4 parentUVmat = parent->GetUVMat();
		ret_uvMat = ret_uvMat.Multiply(parentUVmat);
	}

	else
	{
		ret_uvMat = Get_SRTMat3D({ scale.x, scale.y ,0.0f }, { 0,0,rotate }, { pos.x,pos.y,0.0f });
	}

	return ret_uvMat;

}

UVTransform::UVTransform()
{
	pos = { 0.0f,0.0f };
	scale = { 1.0f,1.0f };
	rotate = 0.0f;

}
