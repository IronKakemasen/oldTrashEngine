#include "../HLSLI/Time.hlsli"
#include "Mt2SzR.hlsli"

//https://www.shadertoy.com/view/Mt2SzR


SamplerState baseColorSmp : register(s0);
SamplerState nomalSmp : register(s1);
SamplerState specularMap : register(s2);
SamplerState shininessMap : register(s3);

ConstantBuffer<Time> gTime : register(b1);

struct VertexShaderOutput
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
    float3 worldPosition : POSITOIN0;
};

struct PixcelShaderOutput
{
    float4 color : SV_TARGET0;
};

PixcelShaderOutput main(VertexShaderOutput input)
{
    PixcelShaderOutput output;
    
    input.texcoord = (input.texcoord + float2(0.5, 0.5)) * 6.0;
    
    float tp = gTime.passedTime;

    float n = nestedNoise(input.texcoord * 6.0, tp * 10.0);
    
    float3 finalColor = lerp(float3(0.5, 0.5, 1.0), float3(0.1, 0.15, 0.1), n);
    
    output.color = float4(finalColor,1.0);
    
    return output;
}

