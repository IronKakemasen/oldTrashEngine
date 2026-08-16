#include "../HLSLI/Material.hlsli"
#include "../HLSLI/CameraPara.hlsli"

Texture2D<float4> colorMap : register(t0);

SamplerState baseColorSmp : register(s0);
SamplerState nomalSmp : register(s1);
SamplerState specularMap : register(s2);
SamplerState shininessMap : register(s3);

ConstantBuffer<Material> gMaterial : register(b1);

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

    float4 transformedUV = mul(float4(input.texcoord.x, input.texcoord.y, 1.0f, 1.0f), gMaterial.uvTransform);
    float4 textureColor = colorMap.Sample(baseColorSmp, transformedUV.xy);
    if (textureColor.a <= 0.0) discard;

    output.color = gMaterial.albedoColor * textureColor;
    
    return output;
}

