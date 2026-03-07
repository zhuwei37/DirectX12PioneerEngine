

#include"Common.hlsl"
#ifndef PBR_LIGHT_HLSL
#define PBR_LIGHT_HLSL
float3 fresnelSchlick(float cosTheta,float3 F0)
{
    float3 F = F0 + (1 - F0) * exp2((-5.55473 * cosTheta - 6.98316) * cosTheta);
    return F;
    //return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}


float DistributionGGX(float NdotH,float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH2 = NdotH * NdotH;
    
    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
    
    return num / denom;
}

float GeometrySchlickGGX(float NdotV,float roughness)
{
    float r = (roughness  + 1.0);
    float k = (r ) * (r ) / 8.0;;
    
    float num = NdotV;
    float denom = NdotV * (1.0 - k) + k;
    return num / denom;
}


float GeometrySmith(float NdotV,float NdotL,float roughness)
{
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);
    return ggx2 * ggx1;
}


float3 BRDF(float3 n, float3 l, float3 v, float roughness, float metalness, float3 albedo,float3 lightColor)
{
    float3 H = normalize(v + l);
    float NdotV = max(dot(n, v), 0.00001);
    float NdotH = max(dot(n, H),0.00001);
    float NdotL = max(dot(n, l),0);
    float HdotV = max(dot(H, v), 0.00001);
    
    float3 F0 = float3(0.04, 0.04, 0.04);
    F0 = lerp(F0, albedo, metalness);
    
    float NDF = DistributionGGX(NdotH, roughness);
    float G = GeometrySmith(NdotV, NdotL, roughness);
    float3 F = fresnelSchlick(HdotV, F0);
    float3 ks = F;
    //漫反射率
    float3 kd = float3(1.0,1.0,1.0) - ks;
    kd *= 1.0 - metalness;
    
    float3 nominator = NDF * G * F;
    float denominator = NdotV * NdotL;
    //镜面反射
    float3 specularColor = nominator * 0.25 / denominator*PI;
    
    float3 diffColor = (kd * albedo )/PI ;
    //return specularColor;
    return ( diffColor+ specularColor) * NdotL*lightColor;
    
}

#endif