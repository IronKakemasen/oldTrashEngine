#include "DirectionalLight.h"
#include "../../Buffer/constantBuffer/DirectionalLightBuffer/DirectionalLightBuffer.h"
#include "../../../../commonVariables.h"
#include "../../../../utilities/benriTemplateFunc/benriTempFunc.h"
void DirectionalLight::Update()
{

}

void DirectionalLight::Init()
{
	para.color = { 255,255,255 };
	para.intensity = 12.0f;
	para.pos = { 1,14,-9.0f };
	para.isActive = 1;
}

void DirectionalLight::Reset()
{

}

DirectionalLight::DirectionalLight()
{

}
