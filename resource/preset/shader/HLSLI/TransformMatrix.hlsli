 #pragma once

struct WorldMatrix
{
    float4x4 World;
};

struct WVPMatrix
{
    float4x4 WVP;
};

struct TransformMatrixBuffer
{
    float4x4 World;
    float4x4 WVP;
};

