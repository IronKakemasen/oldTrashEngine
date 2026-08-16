#pragma once
#include "../bufferAndMap.h"
#include "../../../../../utilities/vectorAndMatrix/vectorAndMatrix.h"

struct SimpleNeonLikePara
{
	Vector2 texelSize;
	float edgeWidth;
	float backgroundDim;

};

struct SimpleNeonLikeBuffer
{
	BufferAndMap<SimpleNeonLikePara> buffer;
};
