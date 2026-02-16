//***************************************************************************************
// Default.hlsl by Frank Luna (C) 2015 All Rights Reserved.
//
// Default shader, currently supports lighting.
//***************************************************************************************

// Defaults for number of lights.

// Include structures and functions for lighting.
#include"Common.hlsl"




// Constant data that varies per frame.


Texture2D gDiffuseMap : register(t0);

Texture2D gNormalMap : register(t1);
// Constant data that varies per material.

cbuffer cbMaterial : register(b3)
{
	float4 gDiffuseAlbedo;
    float3 gFresnelR0;
    float  gRoughness;
};
struct VertexIn
{
	float3 PosL    : POSITION;
    float3 NormalL : NORMAL;
	float2 TexC    : TEXCOORD;
    float3 TangentU : TANGENT;
};

struct VertexOut
{
	float4 PosH    : SV_POSITION;
    float3 PosW    : POSITION;
    float3 NormalW : NORMAL;
    float3 TangentW : TANGENT;
	float2 TexC    : TEXCOORD;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout = (VertexOut)0.0f;
	
    // Transform to world space.
    float4 posW = mul(float4(vin.PosL, 1.0f), gWorld);
    vout.PosW = posW.xyz;

    // Assumes nonuniform scaling; otherwise, need to use inverse-transpose of world matrix.
    vout.NormalW = mul(vin.NormalL, (float3x3)gWorld);

    vout.TangentW = mul(vin.TangentU, (float3x3) gWorld);
    // Transform to homogeneous clip space.
    vout.PosH = mul(posW, gViewProj);
	
	// Output vertex attributes for interpolation across triangle.
    //float4 texC = mul(float4(vin.TexC, 0.0f, 1.0f), gTexTransform);
    //vout.TexC = mul(texC, gMatTransform).xy;
    vout.TexC = vin.TexC.xy;
    return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
    
    float4 diffuseAlbedo = gDiffuseMap.Sample(gsamAnisotropicWrap, pin.TexC) * gDiffuseAlbedo;
    // Interpolating normal can unnormalize it, so renormalize it.
    pin.NormalW = normalize(pin.NormalW);

    // Vector from point being lit to eye. 
    float3 toEyeW = normalize(gEyePosW - pin.PosW);

    // Light terms.
    float4 ambient = gAmbientLight*diffuseAlbedo;

   
    float4 normalMapSample = gNormalMap.Sample(gsamAnisotropicWrap, pin.TexC);
    float3 bumpedNormalW = pin.NormalW;
    if ((normalMapSample.x + normalMapSample.y + normalMapSample.z) > 0.0f)
    {
        bumpedNormalW = NormalSampleToWorldSpace(normalMapSample.rgb, pin.NormalW, pin.TangentW);
    }
    const float shininess = (1.0f - gRoughness) ;
    
    Material mat = { diffuseAlbedo, gFresnelR0, shininess };
    float3 shadowFactor = 1.0f;
    float4 directLight = ComputeLighting(gLights, mat, pin.PosW,
       bumpedNormalW, toEyeW, shadowFactor);

   // return directLight;
    
    //float3 r = reflect(-toEyeW, bumpedNormalW);
   // float3 fresnelFactor = SchlickFresnel(gFresnelR0, bumpedNormalW, r);
    
   float4 litColor = ambient + directLight;
    //float4 litColor = ambient;
    //litColor.rgb += fresnelFactor*shininess;
    // Common convention to take alpha from diffuse material.
    litColor.a = diffuseAlbedo.a;
   
    return litColor;
   // return float4(gFresnelR0,1.0f);
}


