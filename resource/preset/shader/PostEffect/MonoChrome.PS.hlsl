#include "./Conversions/GreyScale.hlsli"

Texture2D<float4> colorMap : register(t0);

SamplerState baseColorSmp : register(s0);
SamplerState nomalSmp : register(s1);
SamplerState specularSmp : register(s2);
SamplerState shininessSmp : register(s3);

struct VertexShaderOutput
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
};

struct PixcelShaderOutput
{
	float4 color : SV_TARGET0;
};

PixcelShaderOutput main(VertexShaderOutput input)
{
    PixcelShaderOutput output;

    float4 textureColor = colorMap.Sample(baseColorSmp, input.texcoord);
    float3 toGrey = GreyScale(textureColor.rgb);
    
    output.color = float4(toGrey, textureColor.a);
    
    return output;
}

