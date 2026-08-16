#pragma once

float4 AvarageBlur(Texture2D<float4> texture_, SamplerState sampler_,
    float2 uv_, float width_, float height_)
{
    static float offsetU = 1.5f / width_;
    static float offsetV = 1.5f / height_;
    
    float4 blurredColor = texture_.Sample(sampler_, uv_ + float2(offsetU, 0.0f));
    blurredColor  = texture_.Sample(sampler_, uv_ + float2(-offsetU, 0.0f));
    blurredColor  = texture_.Sample(sampler_, uv_ + float2(0.0f, offsetV));
    blurredColor  = texture_.Sample(sampler_, uv_ + float2(0.0f, -offsetV));
    blurredColor  = texture_.Sample(sampler_, uv_ + float2(offsetU, offsetV));
    blurredColor  = texture_.Sample(sampler_, uv_ + float2(offsetU, -offsetV));
    blurredColor  = texture_.Sample(sampler_, uv_ + float2(-offsetU, offsetV));
    blurredColor = texture_.Sample(sampler_, uv_ + float2(-offsetU, -offsetV));

    return blurredColor /= 9.0f;
}

