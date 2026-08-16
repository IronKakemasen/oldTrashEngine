#pragma once
#include "../CameraParaBuffer.h"
#include < memory >

//struct ID3D12Device;

class CameraParameterSetter
{
	std::unique_ptr < CameraParaBuffer > cameraParaBuffer;
	void Create(ID3D12Device* device_);

public:
	void Init(ID3D12Device* device_);
	void SetCameraPara(CameraBufferPara cameraPara_);
	inline auto* Getter_Buffer()
	{
		return cameraParaBuffer.get();
	}
};

