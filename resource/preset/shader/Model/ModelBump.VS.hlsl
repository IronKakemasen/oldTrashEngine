#include "../HLSLI/TransformMatrix.hlsli"
#include "../HLSLI/VertexShaderInput.hlsli"

struct VertexShaderOutput
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
    float3 worldPosition : POSITOIN0;
    float3x3 invTangentBasis : INV_TANGENT_BASIS; //接線空間への基底変換行列の逆行列
};

ConstantBuffer<TransformMatrixBuffer> transformMatrix : register(b0);

VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;
  
    float4 tmp = float4(input.position.x, input.position.y, input.position.z, 1.0f);
    
    output.position = mul(tmp, transformMatrix.WVP);
    output.texcoord = input.texcoord;
    output.normal = normalize(mul(input.normal, (float3x3) transformMatrix.World));
    output.worldPosition = input.position.xyz + float3(transformMatrix.World[3][0], transformMatrix.World[3][1], transformMatrix.World[3][2]);
    
    //基底ベクトル
    float3 N = normalize(mul((float3x3) transformMatrix.World, input.normal));
    float3 T = normalize(mul((float3x3) transformMatrix.World, input.tangent));
    float3 B = normalize(cross(N, T));
    
    // 基底変換行列の逆行列.
    output.invTangentBasis = transpose(float3x3(T, B, N));

    return output;
}



