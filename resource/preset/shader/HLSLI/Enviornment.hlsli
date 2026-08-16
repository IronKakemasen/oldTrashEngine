 #pragma once

struct Enviornment
{
    float3 skydomeColor;
    float3 groundColor;
};

float3 CalcAmbient(Enviornment enviornment_ , float3 groundNormal_, float3 N_)
{
    float3 result;

    float t = dot(N_, groundNormal_);
    t = (t + 1.0f) * 0.5f;
    result = lerp(enviornment_.groundColor, enviornment_.skydomeColor, t);
    
    return result;

}
