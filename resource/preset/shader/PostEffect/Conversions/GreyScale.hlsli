 #pragma once

float3 GreyScale(float3 color_)
{
    static const float3 lumaCoeff = float3(0.299f, 0.587f, 0.114f);
    return dot(color_, lumaCoeff);

}