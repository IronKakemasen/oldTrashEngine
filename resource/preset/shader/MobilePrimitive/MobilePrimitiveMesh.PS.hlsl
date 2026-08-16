#include "../HLSLI/Material.hlsli"
#include "../HLSLI/DirectionalLight.hlsli"
#include "../HLSLI/ComputeDiffuse.hlsli"
#include "../HLSLI/CameraPara.hlsli"
#include "../HLSLI/PointLight.hlsli"


Texture2D<float4> colorMap : register(t0);
StructuredBuffer<PointLight> pointLights : register(t1);

SamplerState baseColorSmp : register(s0);
SamplerState nomalSmp : register(s1);
SamplerState specularMap : register(s2);
SamplerState shininessMap : register(s3);

ConstantBuffer<Material> gMaterial : register(b2);
ConstantBuffer<DirectionalLight> dirLight : register(b3);
ConstantBuffer<CameraPara> cameraPara : register(b4);

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

    float4 transformedUV = mul(float4(input.texcoord.x, input.texcoord.y, 1.0f, 1.0f), gMaterial.uvTransform);
    float4 textureColor = colorMap.Sample(baseColorSmp, transformedUV.xy);

    float3 normal = normalize(input.normal);
    float3 toCamera = normalize(cameraPara.cameraPos - input.worldPosition);

    float NV = saturate(dot(normal, toCamera));
    float3 diffuse = DiffuseModelNormalizedLambert(textureColor.rgb, gMaterial.metallic);
    float3 Ks = textureColor.rgb * gMaterial.metallic;
    float a = gMaterial.roughness * gMaterial.roughness;

    float3 lightFinalColor = float3(0, 0, 0);
    
    //直接光

    float3 tmp = input.worldPosition;
    DirectionalLight tmp1 = dirLight;

    lightFinalColor += ComputeDirectionalLight(tmp, toCamera,
        normal, NV, a, Ks, diffuse, tmp1);
        
    //ポイントライト
    uint numLights, stride;
    pointLights.GetDimensions(numLights, stride);
    
    for (uint i = 0; i < numLights; ++i)
    {
        lightFinalColor += ComputePointLight(input.worldPosition, toCamera, normal, NV,
            a, Ks, diffuse, pointLights[i]);
    }
    
    
    output.color = float4(lightFinalColor, textureColor.a);
    
    return output;
}

