#include "../HLSLI/Time.hlsli"
#include "tft3RH.hlsli"

//https://www.shadertoy.com/view/tft3RH


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
    
    input.texcoord = (input.texcoord + float2(0.5, 0.5)) * 8.0;
    
    float tp = gTime.passedTime * 4.0f;
    float tp2 = 0.5 + saturate(sin(3.14 * gTime.passedTime * 0.2));
    float4 finalColor = float4(0, 0, 0, tp2);

    finalColor.rgb = mainImage(finalColor, input.texcoord, tp);
    
    output.color = finalColor;
    
    return output;
}

