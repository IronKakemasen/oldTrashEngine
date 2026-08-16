#include "../HLSLI/MaterialForLine.hlsli"
#include "../HLSLI/CameraPara.hlsli"

SamplerState baseColorSmp : register(s0);
SamplerState nomalSmp : register(s1);
SamplerState specularSmp : register(s2);
SamplerState shininessSmp : register(s3);

ConstantBuffer<MaterialForLine> gMaterial : register(b1);

struct VertexShaderOutput
{
    float4 position : SV_POSITION;
    float3 depthInView : TEXCOORD1;

};

struct PixcelShaderOutput
{
	float4 color : SV_TARGET0;
    float4 depth : SV_TARGET1;
};

PixcelShaderOutput main(VertexShaderOutput input)
{
    PixcelShaderOutput output;
    output.color = gMaterial.albedoColor;
    output.depth = float4(input.depthInView , 0.0f);
 
    return output;
}

