#pragma once
#include "../../utilities/Transform/Transform.h"
#include "../../utilities/Easing/EasingFunctions.h"

struct CameraPara
{
	Transform trans;
	float fov = CommonV::FOVy;
	CameraPara();
};

struct CameraBehavior
{
	Matrix4 world;
	Matrix4 vpMat;
	Matrix4 viewMat;
	virtual void Update() = 0;

protected:
	void SetViewProjectionMat();
	void DirectionInterpolate();
	CameraPara para;

public:

	Matrix4 HHHHHHH();

	inline auto* Getter_Parameters()
	{
		return &para;
	}
};

struct Camera:public CameraBehavior
{

};

