 #pragma once

float3 DiffuseModelNormalizedLambert(float3 baseColor_, float metalic_)
{
    return baseColor_ * (1.0f - metalic_) * 0.31830988618;
}

float3 DiffuseModelLambert(float3 normal_, float3 lightDir_, float3 diffuse_,
    float3 lightColor_, float intensity_)
{
    float cos = saturate(dot(normal_, lightDir_));
    
    return lightColor_ * cos * diffuse_ * intensity_;
}
