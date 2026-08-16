#include "../HLSLI/Material.hlsli"
#include "../HLSLI/DirectionalLight.hlsli"
#include "../HLSLI/ComputeDiffuse.hlsli"
#include "../HLSLI/CameraPara.hlsli"
#include "../HLSLI/PointLight.hlsli"
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

    float3 dirLightDir = normalize(float3(dirLight.pos.x, dirLight.pos.y, dirLight.pos.z));
    float3 normal = normalize(input.normal);
    float3 toCamera = normalize(cameraPara.cameraPos - input.worldPosition);

    float3 H = normalize(toCamera + dirLightDir);
    
    float NH = saturate(dot(normal, H));
    float NV = saturate(dot(normal, toCamera));
    float NL = saturate(dot(normal, dirLightDir));
    float VH = saturate(dot(toCamera, H));
    
    float3 diffuse = DiffuseModelNormalizedLambert(gMaterial.albedoColor.rgb, gMaterial.metallic);
    
    float3 Ks = gMaterial.albedoColor.rgb * gMaterial.metallic;
    float a = gMaterial.roughness * gMaterial.roughness;
    float D = Distribution_Beckmann(a, NH);
    float G2 = ShadowMasking_Vcavity(NH, NV, NL, VH);
    float3 Fr = SchlickFrensnel(H, toCamera, Ks);
        
    float3 specular = (D * G2 * Fr) / (4.0f * NV * NL);
    
    output.color = float4(dirLight.color * dirLight.intensity * textureColor.rgb *
        (diffuse + specular) * NL, gMaterial.albedoColor.a * textureColor.a);
    return output;

}

