#pragma once
#include "../bufferAndMap.h"
#include "../../../../math/vector/vector.h"


struct CameraBufferPara
{
	Vector3 cameraWorld;
	float pad;
};

struct CameraParaBuffer
{
	BufferAndMap<CameraBufferPara> cameraPara;
};


