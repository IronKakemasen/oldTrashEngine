#include "GaussianBlurBuffer.h"
#include <math.h>

void GaussianWeight::Init()
{
    float buffWeight[8] = {};
    CalcWeightsTableFromGaussian(buffWeight, 8, 30.0f);

    int n = 0;
    for (int k = 0; k < 2; ++k)
    {
        for (int i = 0; i < 4; ++i)
        {
            weight[k][i] = buffWeight[n++];
        }
    }
}

void GaussianWeight::CalcWeightsTableFromGaussian(float* weightsTbl, int sizeOfWeightsTbl, float sigma)
{
    // 重みの合計を記録する変数を定義する
    float total = 0;

    // ここからガウス関数を用いて重みを計算している
    // ループ変数のxが基準テクセルからの距離
    for (int x = 0; x < sizeOfWeightsTbl; x++)
    {
        weightsTbl[x] =  expf(-0.5f * (float)(x * x) / sigma);
        total += 2.0f * weightsTbl[x];
    }

    // 重みの合計で除算することで、重みの合計を1にしている
    for (int i = 0; i < sizeOfWeightsTbl; i++)
    {
        weightsTbl[i] /= total;
    }
}
