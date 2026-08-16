 #pragma once

#include "ComputeBRDF.hlsli"

struct PointLight
{
    float3 pos;
    float intensity;
    float3 color;
    float invSqrRadius; 
    int isActive;

};

float SmoothDistanceAttenuation(float sqrtDistance_, float invSqrtRadius_)
{
    float factor = sqrtDistance_ * invSqrtRadius_;
    float smoothFactor = saturate(1.0f - (factor * factor));
    return smoothFactor * smoothFactor;
}

float GetDistanceAttenuation(float3 unnormalizedToObject_,float invSqrtRadius_ )
{
    const float minDistance = 0.01f;
    
    float sqrDist = dot(unnormalizedToObject_, unnormalizedToObject_);
    float attenuation = 1.0f / max(sqrDist, minDistance * minDistance);

    attenuation *= SmoothDistanceAttenuation(sqrDist, invSqrtRadius_);

    return attenuation;
}

float3 EvaluatePointLight(float3 normal_, float3 worldPos_, PointLight pLight_)
{
    float3 dif = pLight_.pos - worldPos_;
    float3 L = normalize(dif);
    float att = GetDistanceAttenuation(dif, pLight_.invSqrRadius);

    static const float tmp = 1.0f / (4.0f * 3.14159265359f);
    return saturate(dot(normal_, L)) * pLight_.color * att * tmp;
}

float3 ComputePointLight(float3 world_, float3 V_, float3 N_, float VN_,
    float squaredRoughness_, float3 Ks_, float3 diffuse_, PointLight light_)
{
    if (!light_.isActive) return float3(0,0,0);

    float3 pointLightDir = normalize(light_.pos - world_);

    float3 poinghtLightColor = EvaluatePointLight(N_, world_,
            light_) * light_.intensity * light_.isActive;
        
    float3 pointLightBRDF = ComputeBRDF(diffuse_, pointLightDir, V_, N_, VN_, Ks_, squaredRoughness_);
 
    return  poinghtLightColor * pointLightBRDF;
}


