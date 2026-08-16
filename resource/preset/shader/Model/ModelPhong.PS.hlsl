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
    float3 color = gMaterial.albedoColor.rgb * textureColor.rgb;

    float3 dirLightDir = normalize(float3(dirLight.pos.x, dirLight.pos.y, dirLight.pos.z));
    float3 normal = normalize(input.normal);
    float3 toCamera = normalize(cameraPara.cameraPos - input.worldPosition);
    
    float3 diffuse = DiffuseModelLambert(normal, dirLightDir, gMaterial.diffuse, dirLight.color, dirLight.intensity);
    float3 specular = SpecularModelPhong(normal, dirLightDir, toCamera, gMaterial.specular,
    dirLight.color, dirLight.intensity, gMaterial.shininess);
    
    output.color = float4(color.rgb * (diffuse + specular), gMaterial.albedoColor.a * textureColor.a);
    return output;

}

