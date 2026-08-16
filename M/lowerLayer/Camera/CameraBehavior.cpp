#include "CameraBehavior.h"
#include "CameraController/CameraController.h"

CameraPara::CameraPara()
{
	fov = CommonV::FOVy;
	trans.interpolationCoe = 0.05f;
	trans.pos = { 0.0f,3.0f,-1.5f };
	trans.lookDir.y = -0.45f;

}

void CameraBehavior::DirectionInterpolate()
{

}

void CameraBehavior::SetViewProjectionMat()
{
	Matrix4 worldMat = para.trans.GetWorldMatrix();
	viewMat = worldMat.GetInversed();
	float aspectRatio = CommonV::kWindow_W / CommonV::kWindow_H;
	Matrix4 projMat = Get_PerspectiveFOVForZReverse(para.fov, aspectRatio);

	vpMat = viewMat.Multiply(projMat);
	world = worldMat;
}

Matrix4 CameraBehavior::HHHHHHH()
{
	Matrix4 worldMat = para.trans.GetWorldMatrix();
	viewMat = worldMat.GetInversed();
	float aspectRatio = CommonV::kWindow_W / CommonV::kWindow_H;
	Matrix4 projMat = Get_PerspectiveFOVForZReverse(para.fov, aspectRatio);
	
	return projMat;

}