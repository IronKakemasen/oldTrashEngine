 #pragma once

#include "ComputeBRDF.hlsli"

struct DirectionalLight
{
    float3 pos;
    float intensity;
    float3 color;
    int isActive;
};


float3 ComputeDirectionalLight(float3 world_, float3 V_, float3 N_, float VN_,
    float squaredRoughness_, float3 Ks_, float3 diffuse_, DirectionalLight dirLight_)
{
    if (dirLight_.isActive == 0) return float3(0, 0, 0);


    float3 dirLightDir = normalize(dirLight_.pos);
    float3 dirColor = dirLight_.color * dirLight_.intensity * dirLight_.isActive;

    float3 dirLightBRDF = ComputeBRDF(diffuse_, dirLightDir, V_, N_, VN_, Ks_, squaredRoughness_);

    return dirColor * dirLightBRDF;

}