#include "../HLSLI/TransformMatrix.hlsli"
#include "../HLSLI/VertexShaderInput.hlsli"

//https://www.shadertoy.com/view/Mt2SzR

struct VertexShaderOutput
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
    float3 worldPosition : POSITOIN0;
};

ConstantBuffer<TransformMatrixBuffer> transformMatrix : register(b0);

VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;
    output.texcoord = input.texcoord;
    float4 tmp = float4(input.position.x, input.position.y, input.position.z, 1.0f);
    
    output.position = mul(tmp, transformMatrix.WVP);
    output.normal = normalize(mul(input.normal, (float3x3) transformMatrix.World));
    output.worldPosition = input.position.xyz + float3(transformMatrix.World[3][0], transformMatrix.World[3][1], transformMatrix.World[3][2]);
    
    return output;
}
