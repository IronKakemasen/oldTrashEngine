 #pragma once

struct VertexShaderInput
{
    float3 position : POSITION0;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
    float3 tangent : TANGENT;
};

struct VertexShaderInputLine
{
    float3 position : POSITION0;
};

struct VertexShaderInputPlane
{
    float3 position : POSITION0;
    float2 texcoord : TEXCOORD0;
};
