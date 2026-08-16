Texture2D<float4> sideBlurredTex : register(t0);
Texture2D<float4> verticalBlurredTex : register(t1);

SamplerState baseColorSmp : register(s0);
SamplerState nomalSmp : register(s1);
SamplerState specularSmp : register(s2);
SamplerState shininessSmp : register(s3);

struct VertexShaderOutput
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
};

struct PixcelShaderOutput
{
    float4 color : SV_TARGET0;
};

float4 main(VertexShaderOutput input) : SV_TARGET
{
    PixcelShaderOutput output;
    
    output.color = verticalBlurredTex.Sample(baseColorSmp, input.texcoord);
    output.color += sideBlurredTex.Sample(baseColorSmp, input.texcoord);

    return output.color;
    
}