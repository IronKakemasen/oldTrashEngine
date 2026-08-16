#include "../HLSLI/Material.hlsli"
#include "../HLSLI/DirectionalLight.hlsli"
#include "../HLSLI/CameraPara.hlsli"
#include "../HLSLI/PointLight.hlsli"
#include "../HLSLI/ComputeDiffuse.hlsli"
#include "../HLSLI/RectLight.hlsli"

Texture2D<float4> colorMap : register(t0);
StructuredBuffer<PointLight> pointLights : register(t1);
StructuredBuffer<RectLight> rectLights : register(t2);

SamplerState baseColorSmp : register(s0);
SamplerState nomalSmp : register(s1);
SamplerState specularMap : register(s2);
SamplerState shininessMap : register(s3);


ConstantBuffer<Material> gMaterial : register(b1);
ConstantBuffer<DirectionalLight> dirLight : register(b2);
ConstantBuffer<CameraPara> cameraPara : register(b3);


struct PixcelShaderOutput
{
    float4 color : SV_TARGET0;
};

struct VertexShaderOutput
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
    float3 worldPosition : POSITOIN0;
};

PixcelShaderOutput main(VertexShaderOutput input)
{
    PixcelShaderOutput output;

    float4 transformedUV = mul(float4(input.texcoord.x, input.texcoord.y, 1.0f, 1.0f), gMaterial.uvTransform);
    float4 textureColor = colorMap.Sample(baseColorSmp, transformedUV.xy);

    float3 dirLightDir = normalize(dirLight.pos);
    float3 normal = normalize(input.normal);
    float3 toCamera = normalize(cameraPara.cameraPos - input.worldPosition);
    float NL = saturate(dot(normal, dirLightDir));
    float3 H = normalize(toCamera + dirLightDir);
    
    float3 diffuse = DiffuseModelNormalizedLambert(gMaterial.albedoColor.rgb, gMaterial.metallic);
    float3 specular =
    SpecularModelNormalizedPhong(gMaterial.albedoColor.rgb, gMaterial.metallic,
    gMaterial.shininess, dirLightDir, normal, toCamera);
    
    specular = SchlickFrensnel(H, toCamera, specular);

    output.color = float4(dirLight.color * dirLight.intensity * textureColor.rgb *
        (diffuse + specular) * NL, gMaterial.albedoColor.a * textureColor.a);
    
    return output;

}

