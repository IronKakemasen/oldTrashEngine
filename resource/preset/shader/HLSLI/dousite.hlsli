 #pragma once

struct CameraParameters
{
    float3 camera_worldPos;
};

void BinaryAlpha(float4 color_, float min_)
{
    if (color_.a <= min_)
    {
        discard;
    }
}

