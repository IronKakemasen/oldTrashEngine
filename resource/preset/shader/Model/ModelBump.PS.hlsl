#include "../HLSLI/Material.hlsli"
#include "../HLSLI/DirectionalLight.hlsli"
#include "../HLSLI/ComputeDiffuse.hlsli"
#include "../HLSLI/CameraPara.hlsli"
#include "../HLSLI/PointLight.hlsli"
#include "../HLSLI/RectLight.hlsli"

Texture2D<float4> colorMap : register(t0);
Texture2D<float4> normalMap : register(t1);
StructuredBuffer<PointLight> pointLights : register(t2);
StructuredBuffer<RectLight> rectLights : register(t3);

SamplerState baseColorSmp : register(s0);
SamplerState nomalSmp : register(s1);
SamplerState specularSmp : register(s2);
SamplerState shininessSmp : register(s3);

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
    float3x3 invTangentBasis : INV_TANGENT_BASIS; //接線空間への基底変換行列の逆行列
};


PixcelShaderOutput main(VertexShaderOutput input)
{
    PixcelShaderOutput output;

    float4 transformedUV = mul(float4(input.texcoord.x, input.texcoord.y, 1.0f, 1.0f), gMaterial.uvTransform);
    float4 textureColor = colorMap.Sample(baseColorSmp, transformedUV.xy);

    float3 toCamera = normalize(cameraPara.cameraPos - input.worldPosition);
    float3 normal = normalMap.Sample(nomalSmp, transformedUV.xy).xyz * 2.0 - 1.0f;
    normal = mul(input.invTangentBasis, normal);

    float4 baseColor = textureColor * gMaterial.albedoColor;
    float NV = saturate(dot(normal, toCamera));
    float3 diffuse = DiffuseModelNormalizedLambert(baseColor.rgb, gMaterial.metallic);
    float3 Ks = baseColor.rgb * gMaterial.metallic;
    float a = gMaterial.roughness * gMaterial.roughness;

    float3 lightFinalColor = float3(0, 0, 0);
    
    //直接光
    lightFinalColor += ComputeDirectionalLight(input.worldPosition, toCamera,
        normal, NV, a, Ks, diffuse, dirLight);

    //ポイントライト
    uint numLights, stride;
    pointLights.GetDimensions(numLights, stride);
    
    for (uint i = 0; i < numLights; ++i)
    {
        lightFinalColor += ComputePointLight(input.worldPosition, toCamera, normal, NV,
            a, Ks, diffuse, pointLights[i]);
    }
 
     //rectLight
    uint numRLights, Rstride;
    rectLights.GetDimensions(numRLights, Rstride);
    
    for (uint p = 0; p < numRLights; ++p)
    {
        lightFinalColor += ComputeRectLight(input.worldPosition, toCamera, normal, NV,
            a, Ks, diffuse, rectLights[p]);
    }


    output.color = float4(lightFinalColor, baseColor.a);
    
    return output;
}

