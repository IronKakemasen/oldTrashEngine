#include "RectLight.h"

void RectLight::Update()
{
	static Vector3 up = { 0,1,0 };
	lookDir = lookDir.GetNormalized();
	para.axisX = (up.GetCross(lookDir)).GetNormalized();
	para.axisY = (lookDir.GetCross(para.axisX)).GetNormalized();
}

void RectLight::Init()
{

}

void RectLight::Reset()
{
	para.isActive = 0;
}

RectLight::RectLight()
{
	lookDir = { 0,0,1 };
}
