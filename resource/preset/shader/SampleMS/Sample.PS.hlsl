SamplerState samp : register(s0);

struct MeshShaderOutput
{
    float4 position : SV_Position;
    float4 color : TEXCOORD0;

};

struct PixelShaderOutput
{
    float4 color : SV_Target0;
};

PixelShaderOutput main(MeshShaderOutput input_)
{
    PixelShaderOutput output;
    
    output.color = input_.color;
    return output;
}