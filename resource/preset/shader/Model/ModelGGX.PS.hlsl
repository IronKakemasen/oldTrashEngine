#include "../HLSLI/Material.hlsli"
#include "../HLSLI/DirectionalLight.hlsli"
#include "../HLSLI/ComputeDiffuse.hlsli"
#include "../HLSLI/CameraPara.hlsli"
#include "../HLSLI/PointLight.hlsli"
#include "../HLSLI/RectLight.hlsli"
#include "../HLSLI/Enviornment.hlsli"


Texture2D<float4> colorMap : register(t0);
StructuredBuffer<PointLight> pointLights : register(t1);
StructuredBuffer<RectLight> rectLights : register(t2);

SamplerState baseColorSmp : register(s0);
SamplerState nomalSmp : register(s1);
SamplerState specularMap : register(s2);
SamplerState shininessMap : register(s3);

ConstantBuffer< Material > gMaterial : register(b1);
ConstantBuffer< DirectionalLight > dirLight : register(b2);
ConstantBuffer< CameraPara > cameraPara : register(b3);
ConstantBuffer< Enviornment > enviornment : register(b4);


struct PixcelShaderOutput
{
    float4 color : SV_TARGET0;
    float4 depth : SV_TARGET1;
};

struct VertexShaderOutput
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
    float3 worldPosition : POSITOIN0;
    float3 depthInView : TEXCOORD1;
};

PixcelShaderOutput main(VertexShaderOutput input)
{
    PixcelShaderOutput output;
 
    output.depth = float4(input.depthInView,1.0f);

    float4 transformedUV = mul(float4(input.texcoord.x, input.texcoord.y, 1.0f, 1.0f), gMaterial.uvTransform);
    float4 textureColor = colorMap.Sample(baseColorSmp, transformedUV.xy);
    float4 baseColor = textureColor * gMaterial.albedoColor;

    if (baseColor.a <= 0.2f) discard;
    
    float3 normal = normalize(input.normal);
    float3 toCamera = normalize(cameraPara.cameraPos - input.worldPosition);

    float NV = saturate(dot(normal, toCamera));
    float3 diffuse = DiffuseModelNormalizedLambert(baseColor.rgb, gMaterial.metallic);
    float3 Ks = baseColor.rgb * gMaterial.metallic;
    float a = gMaterial.roughness * gMaterial.roughness;

    float3 lightFinalColor = float3(0, 0, 0);
    
    float3 tmp = input.worldPosition;
    DirectionalLight tmp1 = dirLight;
    
    //直接光
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
    
    //rectLight
    uint numRLights, Rstride;
    rectLights.GetDimensions(numRLights, Rstride);
    
    for (uint p = 0; p < numRLights; ++p)
    {
        lightFinalColor += ComputeRectLight(input.worldPosition, toCamera, normal, NV,
            a, Ks, diffuse, rectLights[p]);
    }

    Enviornment tmpEnv = enviornment;

    lightFinalColor += CalcAmbient(tmpEnv, float3(0, 1, 0), normal);
    
    output.color = float4(lightFinalColor, baseColor.a);
    
    return output;
}

