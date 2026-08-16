float random(float x)
{
    return frac(sin(x) * 10000.0);
}

float noise(float2 p)
{
    return random(p.x + p.y * 10000.0);
}

float2 sw(float2 p)
{
    return float2(floor(p.x), floor(p.y));
}
float2 se(float2 p)
{
    return float2(ceil(p.x), floor(p.y));
}
float2 nw(float2 p)
{
    return float2(floor(p.x), ceil(p.y));
}
float2 ne(float2 p)
{
    return float2(ceil(p.x), ceil(p.y));
}

float smoothNoise(float2 p)
{
    float2 interp = smoothstep(0.0, 1.0, frac(p));
    float s = lerp(noise(sw(p)), noise(se(p)), interp.x);
    float n = lerp(noise(nw(p)), noise(ne(p)), interp.x);
    return lerp(s, n, interp.y);
}

float fractalNoise(float2 p)
{
    float x = 0.0;
    x += smoothNoise(p);
    x += smoothNoise(p * 2.0) * 0.5;
    x += smoothNoise(p * 4.0) * 0.25;
    x += smoothNoise(p * 8.0) * 0.125;
    x += smoothNoise(p * 16.0) * 0.0625;
    x /= 1.0 + 0.5 + 0.25 + 0.125+ 0.0625;
    return x;
}

float movingNoise(float2 p, float iTime)
{
    float x = fractalNoise(p + iTime);
    float y = fractalNoise(p - iTime);
    return fractalNoise(p + float2(x, y));
}

float nestedNoise(float2 p, float iTime)
{
    float x = movingNoise(p, iTime);
    float y = movingNoise(p + 100.0, iTime);
    return movingNoise(p + float2(x, y), iTime);
}