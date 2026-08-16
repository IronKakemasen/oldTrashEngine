#pragma once

#include "../bufferAndMap.h"
#include "../../../utilities/vectorAndMatrix/vectorAndMatrix.h"

struct EnviornmentParam
{
	Vector3 skydomeColor;
	float pad1{};
	Vector3 groundColor;
	float pad2{};

};

struct EnviornmentBuffer
{
	EnviornmentBuffer();

	BufferAndMap<EnviornmentParam> enviornmentParam;
	static EnviornmentBuffer* Get();

};

