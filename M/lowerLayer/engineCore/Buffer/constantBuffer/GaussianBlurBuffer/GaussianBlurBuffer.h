#pragma once
#include "../bufferAndMap.h"

struct GaussianWeight
{
	float weight[2][4];

	void Init();

private:
	void CalcWeightsTableFromGaussian(float* weightsTbl, int sizeOfWeightsTbl, float sigma);

};

struct GaussianBlurBuffer
{
	BufferAndMap<GaussianWeight> buffMap;
};


