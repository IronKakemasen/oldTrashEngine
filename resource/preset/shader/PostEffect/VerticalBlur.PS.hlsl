#include "./Conversions/GaussianBlur.hlsli"

Texture2D<float4> sceneTexture : register(t1);

SamplerState baseColorSmp : register(s0);
SamplerState nomalSmp : register(s1);
SamplerState specularSmp : register(s2);
SamplerState shininessSmp : register(s3);

ConstantBuffer<GaussianBlurParam> gaussianBlurParam : register(b1);

struct VertexShaderOutput
{
    float4 position : SV_POSITION;
    RoundTexel roundTexel : TEXCOORD0;
};


struct PixcelShaderOutput
{
    float4 color : SV_TARGET0;
};


float4 main(VertexShaderOutput input) : SV_TARGET
{
    PixcelShaderOutput output;
    
    RoundTexel tmpRoundtexel = input.roundTexel;
    GaussianBlurParam tmpGaussianBlurParam = gaussianBlurParam;
   
    float4 finalColor =
    GaussianBlur(sceneTexture, baseColorSmp, tmpGaussianBlurParam, tmpRoundtexel);

    return finalColor;
}