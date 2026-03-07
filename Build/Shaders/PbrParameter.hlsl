#include"Common.hlsl"
#include"PbrLight.hlsl"
float4 PbrParameterLight(float4 rt0,float4 rt1,float4 rt2,float4 rt3,float3 shadow)
{
    float3 worldPos = rt0.xyz;
    float3 normal = normalize(rt1.xyz);
    float roughness = rt1.w;
    float3 albedo = rt2.rgb;
    float metalness = rt2.w;
    
    
    Light L = gLights[0];
    
    float3 lightVec = normalize(-L.Direction);
    float3 viewDir = normalize(gEyePosW - worldPos);
    
    float3 Lo = BRDF(normal, lightVec, viewDir, roughness, metalness, albedo,L.Strength);
    Lo *= shadow[0];
    float4 ambient = float4(albedo, 1) * gAmbientLight;
    
    float3 color = ambient.rgb+Lo;
    color /= (color + 1.0);
    color = pow(color, 1.0 / 2.0);
   
  //  return float4(Lo, 1);
    //return float4(0.1, 0.5, 0.2, 1);
    return float4(color, 1);
    

}