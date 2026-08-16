 #pragma once

float3 SpecularModelPhong(float3 normal_, float3 lightDir_, float3 toCamera_, float3 specular_,
    float3 lightColor_ , float intensity_ , float shininess_)
{
    float3 reflection = normalize(-reflect(toCamera_, normal_));
    return lightColor_ * specular_ * pow(saturate(dot(lightDir_, reflection)), shininess_) * 
        intensity_;   
}

float3 SpecularModelNormalizedPhong(float3 baseColor_ , float metalic_,
    float shininess_, float3 lightDir_, float3 normal_, float3 toCamera_)
{
    float3 reflection = normalize(-toCamera_ + 2.0f * dot(normal_, toCamera_) * normal_);
    float LR = saturate(dot(lightDir_, reflection));
    
    return baseColor_ * metalic_ * ((shininess_ + 2.0f) / (2.0f * 3.14159265359f)) * pow(LR, shininess_);
}

float3 SchlickFrensnel(float3 H_ ,float3 toCamera_ , float3 specularColor_)
{
    float VH = saturate(dot(toCamera_, H_));
    return specularColor_ + (1.0f - specularColor_) * pow(saturate(1.0f - VH), 5.0f);
}


float Distribution_Beckmann(float m_, float NH_)
{
    float c2 = NH_ * NH_;
    float c4 = c2 * c2;
    float m2 = m_ * m_;

    return (1.0f / (m2 * c4)) * exp((-1.0f / m2) * (1.0f - c2) / c2);
}

float Distribution_GGX(float m_, float NH_)
{
    float m2 = m_ * m_;

    float f = (NH_ * m2 - NH_) * NH_ + 1.0f;
    return m2 / max(3.14159265359f * f * f, 0.0000001);
}


float ShadowMasking_Vcavity(float NH_, float NV_, float NL_, float VH_)
{
    return min(1.0f, min(2.0f * NH_ * NV_ / VH_, 2.0f * NH_ * NL_ / VH_));
}

float G2_Smith(float NL_, float NV_, float m_)
{
    float m2 = m_ * m_;

    float NL2 = NL_ * NL_;
    float NV2 = NV_ * NV_;

    float lambdaV = (-1.0f + sqrt(m2 * (1.0f - NL2) / NL2 + 1.0f)) * 0.5f;
    float lambdaL = (-1.0f + sqrt(m2 * (1.0f - NV2) / NV2 + 1.0f)) * 0.5f;

    return 1.0f / (1.0f + lambdaV + lambdaL);

}

float3 ComputeBRDF(float3 diffuse_, float3 lightDir_, float3 toCamera_, float3 normal_,
        float NV_, float3 Ks_, float a_)
{
    float NL = saturate(dot(normal_, lightDir_));
    if (NL <= 0.0) return float3(0, 0, 0);

    float3 H = normalize(toCamera_+ lightDir_);
    float NH = saturate(dot(normal_, H));
    
    float D = Distribution_GGX(a_ , NH);
    float G2 = G2_Smith(NL, NV_, a_);
    float3 Fr = SchlickFrensnel(H, toCamera_, Ks_);
        
    float3 specular = float3((Fr * D * G2 ) / (4.0f * NV_ * NL));
    
    return saturate((diffuse_ + specular) * NL);

}
    



