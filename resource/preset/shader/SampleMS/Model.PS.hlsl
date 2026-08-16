SamplerState samp : register(s0);
Texture2D<float4> colorMap : register(t4);

struct MeshShaderOutput
{
    float4 position : SV_Position;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
    float3 worldPosition : POSITOIN0;
};

struct PixelShaderOutput
{
    float4 color : SV_Target0;
};

PixelShaderOutput main(MeshShaderOutput input_)
{
    PixelShaderOutput output;
    
    float4 textureColor = colorMap.Sample(samp, input_.texcoord);

    output.color = textureColor;
    return output;
}