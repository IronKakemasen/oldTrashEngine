Texture2D<float4> sceneTex : register(t0);
Texture2D<float4> depthOfViewTex : register(t1);
Texture2D<float4> bokeTex : register(t2);

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
    
    float tmp = depthOfViewTex.Sample(baseColorSmp, input.texcoord).z;
    
    clip(tmp - 200.0f);
    
    float4 boke = bokeTex.Sample(baseColorSmp, input.texcoord);
    //boke.b = tmp;
    boke.a = min(1.0f, (tmp - 200.0f) / 500.0f);
   
    return boke;
    
}