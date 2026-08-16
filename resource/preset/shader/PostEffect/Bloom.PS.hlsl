Texture2D<float4> bokeTex0 : register(t0);
Texture2D<float4> bokeTex1 : register(t1);
Texture2D<float4> bokeTex2 : register(t2);
Texture2D<float4> bokeTex3 : register(t3);
Texture2D<float4> sceneTex : register(t4);


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


PixcelShaderOutput main(VertexShaderOutput input)
{
    PixcelShaderOutput output;

    float4 baseColor = sceneTex.Sample(baseColorSmp, input.texcoord);
    
    float4 bloomColor = 
        bokeTex0.Sample(baseColorSmp, input.texcoord );

    bloomColor +=
        bokeTex1.Sample(baseColorSmp, input.texcoord);
    bloomColor +=
        bokeTex2.Sample(baseColorSmp, input.texcoord);
    bloomColor +=
        bokeTex3.Sample(baseColorSmp, input.texcoord);
    
    bloomColor *= 0.5f;
    
    float4 finalColor = baseColor + bloomColor;
        
    
    output.color = float4(finalColor.rgb,1.0f);
    
    return output;
}






