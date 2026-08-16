#pragma once

struct SimpleNeonLike 
{
    float2 texelSize;
    float edgeWidth;
    float backgroundDim; 
};

float CalcLuminance_SNL(float3 c)
{
    return dot(c, float3(0.299, 0.587, 0.114));
}

float3 ConvertToSimpleNeonLike(SimpleNeonLike snl_, float3 color_,float lumaT_,
    float lumaB_, float lumaL_, float lumaR_)
{
    
    float lumaCenter = CalcLuminance_SNL(color_);

    float deltaY = abs(lumaT_ - lumaB_);
    float deltaX = abs(lumaL_ - lumaR_);
    float edge = sqrt(deltaX * deltaX + deltaY * deltaY);

    edge = smoothstep(0.1f, 0.8f, edge);

    float3 neonPart = color_ * edge * 8.0f;
    float3 finalColor = neonPart + (color_ * snl_.backgroundDim);

    return finalColor;
}