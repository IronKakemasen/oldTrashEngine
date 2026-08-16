#include "../HLSLI/Material.hlsli"
#include "../HLSLI/DirectionalLight.hlsli"
#include "../HLSLI/ComputeDiffuse.hlsli"
#include "../HLSLI/CameraPara.hlsli"
#include "../HLSLI/PointLight.hlsli"
#include "../HLSLI/RectLight.hlsli"
#include "../HLSLI/Enviornment.hlsli"


TextureCube<float4> colorMap : register(t0);
StructuredBuffer<PointLight> pointLights : register(t1);
StructuredBuffer<RectLight> rectLights : register(t2);

SamplerState baseColorSmp : register(s0);
SamplerState nomalSmp : register(s1);
SamplerState specularMap : register(s2);
SamplerState shininessMap : register(s3);

ConstantBuffer<Material> gMaterial : register(b1);
ConstantBuffer<DirectionalLight> dirLight : register(b2);
ConstantBuffer<CameraPara> cameraPara : register(b3);
ConstantBuffer<Enviornment> enviornment : register(b4);


struct PixcelShaderOutput
{
    float4 color : SV_TARGET0;
    float4 depth : SV_TARGET1;
};

struct VertexShaderOutput
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
    float3 tex3 : NORMAL0;
    float3 worldPosition : POSITOIN0;
    float3 depthInView : TEXCOORD1;
};

PixcelShaderOutput main(VertexShaderOutput input)
{
    PixcelShaderOutput output;
 
    output.depth = float4(input.depthInView, 1.0f);

    float4 textureColor = colorMap.Sample(baseColorSmp, input.tex3);
    float4 baseColor = textureColor * gMaterial.albedoColor;

    if (baseColor.a <= 0.2f) discard;
    
    output.color = baseColor;
    
    return output;
}

