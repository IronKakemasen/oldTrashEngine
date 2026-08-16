#include "./Conversions/SimpleNeonLike.hlsli"

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

ConstantBuffer<SimpleNeonLike> snl : register(b1);

PixcelShaderOutput main(VertexShaderOutput input)
{
    PixcelShaderOutput output;
    float4 textureColor = colorMap.Sample(baseColorSmp, input.texcoord);
    float3 finColor = float3(0,0,0);
    
    float2 offset = snl.texelSize * snl.edgeWidth;

    float lumaT = CalcLuminance_SNL(colorMap.Sample(baseColorSmp, input.texcoord + float2(0, -offset.y)).rgb);
    float lumaB = CalcLuminance_SNL(colorMap.Sample(baseColorSmp, input.texcoord + float2(0, offset.y)).rgb);
    float lumaL = CalcLuminance_SNL(colorMap.Sample(baseColorSmp, input.texcoord + float2(-offset.x, 0)).rgb);
    float lumaR = CalcLuminance_SNL(colorMap.Sample(baseColorSmp, input.texcoord + float2(offset.x, 0)).rgb);

    SimpleNeonLike tmpKun;
    tmpKun = snl;
    
    output.color.rgb = ConvertToSimpleNeonLike(tmpKun, textureColor.rgb,
        lumaT, lumaB, lumaL, lumaR);
    
    output.color.a = 1.0f;
   
    
    return output;
}

