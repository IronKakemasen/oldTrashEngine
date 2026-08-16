#include "LightBehavior.h"

PointLightPara::PointLightPara()
{
	isActive = 0;
	invSqrRadius = 5.0f;
	color = { 255,255,255 };
	intensity = 100.0f;
	pos = { 0.1f,2.4f,2.2f };

}

RectLightPara::RectLightPara()
{
	intensity = 10.0f;
	color = {255,255,255};
	attenuationRadius = 10.0f;
	axisX = {1,0,0};
	width = 1.0f;
	axisY = {0,1,0};
	height = 1.0f;
	halfSize = { width * 0.5f ,height * 0.5f };
	isActive = 0;
}

LightBasePara::LightBasePara()
{
	isActive = 0;

}
