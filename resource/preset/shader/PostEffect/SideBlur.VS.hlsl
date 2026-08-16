#include "../HLSLI/TransformMatrix.hlsli"
#include "../HLSLI/VertexShaderInput.hlsli"
#include "./Conversions/GaussianBlur.hlsli"


struct VertexShaderOutput
{
    float4 position : SV_POSITION;
    RoundTexel roundTexel : TEXCOORD0;
    float2 uv : TEXCOORD8;

    //float4 tex0 : TEXCOORD0;
    //float4 tex1 : TEXCOORD1;
    //float4 tex2 : TEXCOORD2;
    //float4 tex3 : TEXCOORD3;
    //float4 tex4 : TEXCOORD4;
    //float4 tex5 : TEXCOORD5;
    //float4 tex6 : TEXCOORD6;
    //float4 tex7 : TEXCOORD7;

};

Texture2D<float4> sceneTexture : register(t0);

ConstantBuffer<WVPMatrix> gWVPMatrix : register(b0);

VertexShaderOutput main(VertexShaderInputPlane input)
{
    VertexShaderOutput output;
    
    float4 tmp = float4(input.position.x, input.position.y, input.position.z, 1.0f);
    output.position = mul(tmp,gWVPMatrix.WVP);
    output.uv = input.texcoord;

    float2 texSize;
    float level;
    sceneTexture.GetDimensions(0, texSize.x, texSize.y, level);

    float2 baseTex = input.texcoord;

    output.roundTexel.tex0.xy = float2(1.0f / texSize.x, 0.0f);
    output.roundTexel.tex1.xy = float2(3.0f / texSize.x, 0.0f);
    output.roundTexel.tex2.xy = float2(5.0f / texSize.x, 0.0f);
    output.roundTexel.tex3.xy = float2(7.0f / texSize.x, 0.0f);
    output.roundTexel.tex4.xy = float2(9.0f / texSize.x, 0.0f);
    output.roundTexel.tex5.xy = float2(11.0f / texSize.x, 0.0f);
    output.roundTexel.tex6.xy = float2(13.0f / texSize.x, 0.0f);
    output.roundTexel.tex7.xy = float2(15.0f / texSize.x, 0.0f);

    output.roundTexel.tex0.zw = output.roundTexel.tex0.xy * -1.0f;
    output.roundTexel.tex1.zw = output.roundTexel.tex1.xy * -1.0f;
    output.roundTexel.tex2.zw = output.roundTexel.tex2.xy * -1.0f;
    output.roundTexel.tex3.zw = output.roundTexel.tex3.xy * -1.0f;
    output.roundTexel.tex4.zw = output.roundTexel.tex4.xy * -1.0f;
    output.roundTexel.tex5.zw = output.roundTexel.tex5.xy * -1.0f;
    output.roundTexel.tex6.zw = output.roundTexel.tex6.xy * -1.0f;
    output.roundTexel.tex7.zw = output.roundTexel.tex7.xy * -1.0f;

    output.roundTexel.tex0 += float4(baseTex, baseTex);
    output.roundTexel.tex0.x = clamp(output.roundTexel.tex0.x, 0.0f, 1.0f);
    output.roundTexel.tex0.y = clamp(output.roundTexel.tex0.y, 0.0f, 1.0f);
    output.roundTexel.tex0.z = clamp(output.roundTexel.tex0.z, 0.0f, 1.0f);
    output.roundTexel.tex0.w = clamp(output.roundTexel.tex0.w, 0.0f, 1.0f);

    output.roundTexel.tex1 += float4(baseTex, baseTex);
    output.roundTexel.tex1.x = clamp(output.roundTexel.tex1.x, 0.0f, 1.0f);
    output.roundTexel.tex1.y = clamp(output.roundTexel.tex1.y, 0.0f, 1.0f);
    output.roundTexel.tex1.z = clamp(output.roundTexel.tex1.z, 0.0f, 1.0f);
    output.roundTexel.tex1.w = clamp(output.roundTexel.tex1.w, 0.0f, 1.0f);

    output.roundTexel.tex2 += float4(baseTex, baseTex);
    output.roundTexel.tex2.x = clamp(output.roundTexel.tex2.x, 0.0f, 1.0f);
    output.roundTexel.tex2.y = clamp(output.roundTexel.tex2.y, 0.0f, 1.0f);
    output.roundTexel.tex2.z = clamp(output.roundTexel.tex2.z, 0.0f, 1.0f);
    output.roundTexel.tex2.w = clamp(output.roundTexel.tex2.w, 0.0f, 1.0f);

    output.roundTexel.tex3 += float4(baseTex, baseTex);
    output.roundTexel.tex3.x = clamp(output.roundTexel.tex3.x, 0.0f, 1.0f);
    output.roundTexel.tex3.y = clamp(output.roundTexel.tex3.y, 0.0f, 1.0f);
    output.roundTexel.tex3.z = clamp(output.roundTexel.tex3.z, 0.0f, 1.0f);
    output.roundTexel.tex3.w = clamp(output.roundTexel.tex3.w, 0.0f, 1.0f);

    output.roundTexel.tex4 += float4(baseTex, baseTex);
    output.roundTexel.tex4.x = clamp(output.roundTexel.tex4.x, 0.0f, 1.0f);
    output.roundTexel.tex4.y = clamp(output.roundTexel.tex4.y, 0.0f, 1.0f);
    output.roundTexel.tex4.z = clamp(output.roundTexel.tex4.z, 0.0f, 1.0f);
    output.roundTexel.tex4.w = clamp(output.roundTexel.tex4.w, 0.0f, 1.0f);

    output.roundTexel.tex5 += float4(baseTex, baseTex);
    output.roundTexel.tex5.x = clamp(output.roundTexel.tex5.x, 0.0f, 1.0f);
    output.roundTexel.tex5.y = clamp(output.roundTexel.tex5.y, 0.0f, 1.0f);
    output.roundTexel.tex5.z = clamp(output.roundTexel.tex5.z, 0.0f, 1.0f);
    output.roundTexel.tex5.w = clamp(output.roundTexel.tex5.w, 0.0f, 1.0f);

    output.roundTexel.tex6 += float4(baseTex, baseTex);
    output.roundTexel.tex6.x = clamp(output.roundTexel.tex6.x, 0.0f, 1.0f);
    output.roundTexel.tex6.y = clamp(output.roundTexel.tex6.y, 0.0f, 1.0f);
    output.roundTexel.tex6.z = clamp(output.roundTexel.tex6.z, 0.0f, 1.0f);
    output.roundTexel.tex6.w = clamp(output.roundTexel.tex6.w, 0.0f, 1.0f);

    output.roundTexel.tex7 += float4(baseTex, baseTex);
    output.roundTexel.tex7.x = clamp(output.roundTexel.tex7.x, 0.0f, 1.0f);
    output.roundTexel.tex7.y = clamp(output.roundTexel.tex7.y, 0.0f, 1.0f);
    output.roundTexel.tex7.z = clamp(output.roundTexel.tex7.z, 0.0f, 1.0f);
    output.roundTexel.tex7.w = clamp(output.roundTexel.tex7.w, 0.0f, 1.0f);

    return output;
}

