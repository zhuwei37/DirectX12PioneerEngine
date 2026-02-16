
#include"Common.hlsl"
static const float MIN_ROUGHNESS = 0.0000001f;
static const float F0_NON_METAL = 0.04f;
//#include"LightingUtil.hlsl"
struct VertexOut
{
    float4 PosH : SV_POSITION;
    float2 TexC : TEXCOORD;
};
Texture2D RT0 : register(t0);
Texture2D RT1 : register(t1);
Texture2D RT2 : register(t2);
Texture2D RT3 : register(t3);
Texture2D gShadowMap : register(t4);

float DiffusePBR(float3 normal,float3 dirToLight)
{
    return saturate(dot(normal, dirToLight));
}
float3 DiffuseEnergyConserve(float diffuse,float3 specular,float metalness)
{
    return diffuse * ((1 - saturate(specular)) * (1 - metalness));
}

float SpecDistribution(float3 n,float3 h,float roughness)
{
    float NdotH = saturate(dot(n, h));
    float NdotH2 = NdotH * NdotH;
    float a = roughness * roughness;
    float a2 = max(a * a, MIN_ROUGHNESS);
    
    float denomToSquare = NdotH2 * (a2 - 1) + 1;
    return a2 / (PI * denomToSquare * denomToSquare);

}
float GeometricShadowing(float3 n,float3 v,float roughness)
{
    float k = pow((roughness + 1), 2) / 8.0f;
    float NdotV = saturate(dot(n, v));
    
    return NdotV / (NdotV * (1 - k) + k);

}

float3 Fresnel(float3 v,float3 h,float3 f0)
{
    float VdotH = saturate(dot(v, h));
   
    return f0 + (1 - f0) * pow(1 - VdotH, 5);

}
float3 MicrofacetBRDF(float3 n,float3 l,float3 v,float roughness,float metalness,float3 specColor,out float3 kS)
{
    float3 h = normalize(v + l);
    float3 NdotL = saturate(dot(n, l));
    float NdotV = saturate(dot(n, v));
    float D = SpecDistribution(n, h, roughness);
    float3 F = Fresnel(v, h, specColor);
    float G = GeometricShadowing(n, v, roughness) * GeometricShadowing(n, l, roughness);
    kS = F;
    return (D * F * G) / (4 * NdotL * NdotV);

}

float GeomertySchlickGGX(float NdotV,float k)
{
    float nom = NdotV;
    float denom = NdotV * (1.0 - k) + k;
    return nom / denom;

}
float GeomertySmith(float3 N,float3 V,float3 L,float k)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx1 = GeomertySchlickGGX(NdotV, k);
    float ggx2 = GeomertySchlickGGX(NdotL, k);
    return ggx1 * ggx2;
}

float4   PBR( Light light, float3 worldPos,float4 rt1,float4 rt2,float4 rt3 )
{
    //法线
    float3 normal = rt1.rgb;
    //粗糙度
    float roughness = rt1.a;
    //基础颜色
    float3 baseColor = rt2.rgb;
    //金属度
    float metallic = rt2.a;
    //光源方向
    float3 lightdir = normalize(-light.Direction);
    //观察方向
    float3 viewdir = normalize(gEyePosW - worldPos);
    //半方向
    float3 halfdir = normalize(viewdir + lightdir);
    
    //光线强度 朗
    float lightStrangedotL = max(dot(normal, lightdir), 0.0f);
    //光的颜色
    float3 lightStrength = light.Strength;
    //BRDF
    
    float3 kS = float3(0.f, 0.f, 0.f);
    //F0
    float3 specularColor = float3(0.04f, 0.04f, 0.04f);
    specularColor = lerp(specularColor, baseColor, metallic);
    //镜面反射
    float3 spec = MicrofacetBRDF(normal, lightdir, viewdir, roughness, metallic, specularColor, kS);
    
    // 8. 漫反射部分（金属度控制能量分配）
    float diff = DiffusePBR(normal, lightdir);
    float3 balancedDiff = DiffuseEnergyConserve(diff, spec, metallic);
    
    
    float3 kD = (float3(1.0, 1.0, 1.0) - kS) * (1.0 - metallic);
    float3 diffuse = kD * baseColor / PI;
    float3 color = (diffuse + spec) * lightStrangedotL * light.Strength;
    color += baseColor ;
    return float4(color, 1.0f);

}


float CalcShadowFactor(float4 shadowPosH)
{
    float depth = shadowPosH.z;

    uint width, height, numMips;
    gShadowMap.GetDimensions(0, width, height, numMips);

    // Texel size.
    float dx = 1.0f / (float) width;

    float percentLit = 0.0f;
    const float2 offsets[9] =
    {
        float2(-dx, -dx), float2(0.0f, -dx), float2(dx, -dx),
        float2(-dx, 0.0f), float2(0.0f, 0.0f), float2(dx, 0.0f),
        float2(-dx, +dx), float2(0.0f, +dx), float2(dx, +dx)
    };

    [unroll]
    for (int i = 0; i < 9; ++i)
    {
        
        percentLit += gShadowMap.SampleCmpLevelZero(gsamShadow,
            shadowPosH.xy + offsets[i], depth).r;
    }
    
    return percentLit / 9.0f;

}

float4 PSMain(VertexOut pin):SV_Target
{
    float4 worldPosAndMatIndex = RT0.Sample(gsamLinearWrap, pin.TexC);
    float4 NormalAndRoughness = RT1.Sample(gsamLinearWrap, pin.TexC);
    float4 AlbedoAndMetallic = RT2.Sample(gsamLinearWrap, pin.TexC);
    float4 ApecularAndEmissive = RT3.Sample(gsamLinearWrap, pin.TexC);
    float matIndex = worldPosAndMatIndex.a;
    float3 worldPos = worldPosAndMatIndex.rgb;
    if (matIndex == 1)
    {
       
        float3 normal = NormalAndRoughness.rgb;
        float roughness = NormalAndRoughness.a;
        float4 diffuseAlbedo = AlbedoAndMetallic;
        float3 frensnelR0 = ApecularAndEmissive.rgb;
        const float shininess = (1.0f - roughness);
        float3 toEyeW = normalize(gEyePosW - worldPos);
        
        
        float4 ambient = gAmbientLight * diffuseAlbedo;
        Material mat = { diffuseAlbedo, frensnelR0, shininess };
        float3 shadowFactor = float3(1.0f,1.0f,1.0f);
       // float
        float4 ShadowPosH = mul(float4(worldPos, 1.0f), gShadowTransform);
        shadowFactor[0] = CalcShadowFactor(ShadowPosH);
        float4 directLight = ComputeLighting(gLights, mat, worldPos,
        normal, toEyeW, shadowFactor);
       // return directLight;
        float4 litColor = directLight + ambient;
        litColor.a = diffuseAlbedo.a;
        //return ApecularAndEmissive;
        return litColor;
    }
    else if (matIndex == 2)
    {
       // return float4(0.2f,0.5f,0.2f,1.0f);
        return PBR(gLights[0], worldPos, NormalAndRoughness, AlbedoAndMetallic, ApecularAndEmissive);

    }
    else if (matIndex==3)
    {
        float3 normal = NormalAndRoughness.rgb;
        float4 diffuseAlbedo = AlbedoAndMetallic;
        float3 frensnelR0 = float3(0.4f,0.4f,0.4f);
        const float shininess = (1.0f - 0.4);
        float3 toEyeW = normalize(gEyePosW - worldPos);
        
        
        float4 ambient = gAmbientLight * diffuseAlbedo;
        Material mat = { diffuseAlbedo, frensnelR0, shininess };
        float3 shadowFactor = float3(1.0f, 1.0f, 1.0f);
       // float
        float4 ShadowPosH = mul(float4(worldPos, 1.0f), gShadowTransform);
        shadowFactor[0] = CalcShadowFactor(ShadowPosH);
        float4 directLight = ComputeLighting(gLights, mat, worldPos,
        normal, toEyeW, shadowFactor);
       // return directLight;
        float4 litColor = directLight + ambient;
        litColor.a = diffuseAlbedo.a;
        //return ApecularAndEmissive;
        return litColor;
    }
        return float4(0.0f, 0.0f, 0.0f, 0.0f);
      

}