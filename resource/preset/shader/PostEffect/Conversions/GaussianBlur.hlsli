#pragma once

struct GaussianBlurParam
{
    float2x4 weight;
};

struct RoundTexel
{
    float4 tex0;
    float4 tex1;
    float4 tex2;
    float4 tex3;
    float4 tex4;
    float4 tex5;
    float4 tex6;
    float4 tex7;
};

float4 GaussianBlur(Texture2D<float4> texture_, SamplerState sampler_,
    GaussianBlurParam gaussianBlurParam_, RoundTexel roundTexel_)
{
    
    float4 sideBlurredColor = gaussianBlurParam_.weight[0][0] * texture_.Sample(sampler_, roundTexel_.tex0.xy);
    sideBlurredColor += gaussianBlurParam_.weight[0][1] * texture_.Sample(sampler_, roundTexel_.tex1.xy);
    sideBlurredColor += gaussianBlurParam_.weight[0][2] * texture_.Sample(sampler_, roundTexel_.tex2.xy);
    sideBlurredColor += gaussianBlurParam_.weight[0][3] * texture_.Sample(sampler_, roundTexel_.tex3.xy);
    sideBlurredColor += gaussianBlurParam_.weight[1][0] * texture_.Sample(sampler_, roundTexel_.tex4.xy);
    sideBlurredColor += gaussianBlurParam_.weight[1][1] * texture_.Sample(sampler_, roundTexel_.tex5.xy);
    sideBlurredColor += gaussianBlurParam_.weight[1][2] * texture_.Sample(sampler_, roundTexel_.tex6.xy);
    sideBlurredColor += gaussianBlurParam_.weight[1][3] * texture_.Sample(sampler_, roundTexel_.tex7.xy);
    
    sideBlurredColor += gaussianBlurParam_.weight[0][0] * texture_.Sample(sampler_, roundTexel_.tex0.zw);
    sideBlurredColor += gaussianBlurParam_.weight[0][1] * texture_.Sample(sampler_, roundTexel_.tex1.zw);
    sideBlurredColor += gaussianBlurParam_.weight[0][2] * texture_.Sample(sampler_, roundTexel_.tex2.zw);
    sideBlurredColor += gaussianBlurParam_.weight[0][3] * texture_.Sample(sampler_, roundTexel_.tex3.zw);
    sideBlurredColor += gaussianBlurParam_.weight[1][0] * texture_.Sample(sampler_, roundTexel_.tex4.zw);
    sideBlurredColor += gaussianBlurParam_.weight[1][1] * texture_.Sample(sampler_, roundTexel_.tex5.zw);
    sideBlurredColor += gaussianBlurParam_.weight[1][2] * texture_.Sample(sampler_, roundTexel_.tex6.zw);
    sideBlurredColor += gaussianBlurParam_.weight[1][3] * texture_.Sample(sampler_, roundTexel_.tex7.zw);

    return float4(sideBlurredColor.xyz,1.0f);
}
