 #pragma once

struct Material
{
    float3 diffuse;
    float roughness;
    float3 specular;
    float metallic;
    float4 albedoColor;
    float4x4 uvTransform;
    float shininess;
};



