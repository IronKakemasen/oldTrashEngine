#pragma once

#define pi  3.14159
#define tau 6.28318
#define rot(a) mat2(cos(a), -sin(a), sin(a), cos(a))

float2 random2(float2 p)
{
    return frac(sin(float2(dot(p, float2(127.1, 311.7)), dot(p, float2(269.5, 183.3)))) * 43758.5453);
}

static const float timeDiv = 100.5;

float voronoi(float2 uv, float time_)
{
    float2 cell = floor(uv);
    float2 fra = frac(uv);
    float ret = 1.;
    float change = time_ / timeDiv;

    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            float2 neighbor = float2(float(i), float(j));
            float2 rand = random2(cell + neighbor);
            float t = time_ * floor(2.0 * sin(time_));
            rand = 0.5 + 0.5 * sin(change * 4.0 + 2.0 * pi * rand);
            float2 toCenter = neighbor + rand - fra;
            ret = min(ret, max(abs(toCenter.x), abs(toCenter.y)));
        }
    }
    
    return ret;
}

float2 gradient(in float2 x, float thickness, float time_)
{
    float2 h = float2(thickness, 0.0);
    return float2(voronoi(x + h.xy, time_) - voronoi(x - h.xy, time_),
               voronoi(x + h.yx, time_) - voronoi(x - h.yx, time_)) / (0.9 * h.x);
}

float3 mainImage(float4 fragColor, float2 fragCoord,float time_)
{
    fragCoord *= 3.85;

    float val = voronoi(fragCoord, time_) / length(gradient(fragCoord, 0.0235, time_));
    float colVal = pow(val, 3.0) * 3.9;
    
    return lerp(float3(0.0 + colVal, 0.3 + colVal, 0.0 + colVal),
                        float3(0.0, 0, 0.6),
                        clamp(0.9, 0.0, 0.4));

}