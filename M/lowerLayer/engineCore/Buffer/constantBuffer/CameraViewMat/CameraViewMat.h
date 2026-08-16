#pragma once
#include "../bufferAndMap.h"
#include "../../../utilities/vectorAndMatrix/vectorAndMatrix.h"



struct CameraViewMat
{
	CameraViewMat();

	BufferAndMap<Matrix4> viewMat;
	static CameraViewMat* Get();

};

