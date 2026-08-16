#include "../HLSLI/TransformMatrix.hlsli"
#include "../HLSLI/VertexShaderInput.hlsli"

struct VertexShaderOutput
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
    float3 worldPosition : POSITOIN0;
};

StructuredBuffer<TransformMatrixBuffer> gTransformMatrices : register(t0);


VertexShaderOutput main(VertexShaderInput input , uint instanceId : SV_InstanceID)
{
    VertexShaderOutput output;
    
    float4 tmp = float4(input.position.x, input.position.y, input.position.z, 1.0f);
    
    output.position = mul(tmp, gTransformMatrices[instanceId].WVP);
    output.normal = normalize(mul(input.normal, (float3x3) gTransformMatrices[instanceId].World));
    output.texcoord = input.texcoord;
    output.worldPosition = float3(gTransformMatrices[instanceId].World[3][0],
    gTransformMatrices[instanceId].World[3][1], gTransformMatrices[instanceId].World[3][2]);
    
    return output;
}



