 #pragma once

#include "ComputeBRDF.hlsli"

struct RectLight
{
    float3 pos;
    float intensity;
    float3 color;
    float attenuationRadius;
    float3 axisX;
    float width;
    float3 axisY;
    float height;
    float2 halfSize;
    int isActive;
};


float3 CalcIntersectionOnPlane(float3 wPos_, float3 reflection_, float3 lightPos_, float3 lightNormal_)
{
    float denom = dot(lightNormal_, reflection_);

    if (abs(denom) < 1e-6)
    {
        return wPos_ + reflection_ * 10000.0;
    }
    
    float dist = dot(lightNormal_, lightPos_ - wPos_) / denom;
    return wPos_ + reflection_ * dist;
}

float3 GetClosestPointOnRect(float3 targetPoint_, RectLight light_)
{
    float3 lightToPoint = targetPoint_ - light_.pos;

    float distX = dot(lightToPoint, light_.axisX);
    float distY = dot(lightToPoint, light_.axisY);

    distX = clamp(distX, -light_.halfSize.x, light_.halfSize.x);
    distY = clamp(distY, -light_.halfSize.y, light_.halfSize.y);

    return light_.pos + light_.axisX * distX + light_.axisY * distY;
}

float GetRectLightAttenuation(float dist_, float radius_)
{
    float d = max(dist_, 0.0001);
    float att = 1.0 / (d * d);
    
    float distSq = d * d;
    float radiusSq = radius_ * radius_;
    float factor = distSq / radiusSq;
    float smoothFactor = saturate(1.0 - factor * factor);
    return att * smoothFactor * smoothFactor;
}

float3 ComputeRectLight(float3 world_, float3 V_, float3 N_,float VN_,
    float squaredRoughness_, float3 Ks_, float3 diffuse_, RectLight light_)
{
    if (light_.isActive == 0) return float3(0, 0, 0);
    
    float3 lightNormal = cross(light_.axisX, light_.axisY);
    float3 lightColor = light_.color * light_.intensity * light_.isActive;

    float3 R = reflect(-V_, N_);
    
    float3 intersectionPoint = 
        CalcIntersectionOnPlane(world_, R, light_.pos, lightNormal);

    float3 closestPointSpecular = GetClosestPointOnRect(intersectionPoint, light_);
    float3 virtualLightVector_Spec = closestPointSpecular - world_;
    float dist_spec = length(virtualLightVector_Spec);
    float3 L = virtualLightVector_Spec / dist_spec;

    float rectRadius = max(light_.halfSize.x, light_.halfSize.y);
    float subtendedAngle = saturate(rectRadius / dist_spec);
        
    float3 closestPointDiff = GetClosestPointOnRect(world_, light_);
    float3 virtualLightVector_Diff = closestPointDiff - world_;
    float distDiff = length(virtualLightVector_Diff);
    float3 L_diff = virtualLightVector_Diff / distDiff;
    
    float NL_diff = saturate(dot(N_, L_diff));

    float attenuation = GetRectLightAttenuation(distDiff, light_.attenuationRadius);
    float lightFacing = saturate(dot(-L_diff, lightNormal));
    lightColor = lightColor * attenuation * lightFacing * NL_diff;
    
    float alphaPrime = saturate(squaredRoughness_ + subtendedAngle);
    float modifiedRoughness = sqrt(alphaPrime);

    float3 BRDF = ComputeBRDF(diffuse_, L, V_, N_, VN_, Ks_, modifiedRoughness);
    
    return lightColor * BRDF;

}