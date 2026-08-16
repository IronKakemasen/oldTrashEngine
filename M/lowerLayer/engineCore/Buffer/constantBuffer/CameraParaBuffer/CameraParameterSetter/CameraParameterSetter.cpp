#include "CameraParameterSetter.h"

void CameraParameterSetter::Init(ID3D12Device* device_)
{
	static bool onlyOnce = true;
	if (onlyOnce)
	{
		cameraParaBuffer.reset(new CameraParaBuffer);
		Create(device_);
		onlyOnce = false;
	}
}

void CameraParameterSetter::Create(ID3D12Device* device_)
{
	cameraParaBuffer->cameraPara.CreateAndMapping(device_);
}

void CameraParameterSetter::SetCameraPara(CameraBufferPara cameraPara_)
{
	*cameraParaBuffer->cameraPara.buffMap = cameraPara_;
}
