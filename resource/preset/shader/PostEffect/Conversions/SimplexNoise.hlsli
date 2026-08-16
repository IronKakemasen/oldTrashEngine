#pragma once

float hash(float n)
{
    return frac(sin(n) * 43758.5453);
}

float SimplexNoise(float3 x)
{
    float3 p = floor(x);
    float3 f = frac(x);

    f = f * f * (3.0 - 2.0 * f);
    float n = p.x + p.y * 57.0 + 113.0 * p.z;

    return lerp(lerp(lerp(hash(n + 0.0), hash(n + 1.0), f.x),
                     lerp(hash(n + 57.0), hash(n + 58.0), f.x), f.y),
                lerp(lerp(hash(n + 113.0), hash(n + 114.0), f.x),
                     lerp(hash(n + 170.0), hash(n + 171.0), f.x), f.y), f.z);
}

float2 GetNoisedUV(float2 texcoord_,float width_ , float height_,float noiseRate_)
{
    float3 t = float3(texcoord_.x * width_, texcoord_.y * height_, 0.0f);
    float tmp = SimplexNoise(t);
    tmp = (tmp - 0.5f) * 2.0f;
    float2 uv = texcoord_.xy + tmp * noiseRate_;
    
    return uv;
}
