
#include"Common.hlsl"


Texture2D gDiffuseMap : register(t0);

Texture2D gNormalMap : register(t1);
// Constant data that varies per material.

cbuffer cbMaterial : register(b3)
{
    float4 gDiffuseAlbedo;
    float3 gFresnelR0;
    float gRoughness;
};

struct VertexOut
{
    float4 PosH : SV_Position;
    float3 PosW : POSITION;
    float3 NormalW : NORMAL;
    float3 TangentW : TANGENT;
    float2 TexC : TEXCOORD;
};
struct PixelOut
{
    float4 WorldPosMatIndex : SV_Target0;
    float4 NormalRoughness : SV_Target1;
    float4 AlbedoAndMetallic : SV_Target2;
    float4 ApecularAndEmissive : SV_Target3;
};


PixelOut PSMain(VertexOut pin)
{
    float4 diffuseAlbedo = gDiffuseMap.Sample(gsamAnisotropicWrap, pin.TexC);
  
    float4 normalMapSample = gNormalMap.Sample(gsamAnisotropicWrap, pin.TexC);
    float3 bumpedNormalW = normalize(pin.NormalW);
    if ((normalMapSample.x + normalMapSample.y + normalMapSample.z) > 0.0f)
    {
        bumpedNormalW = NormalSampleToWorldSpace(normalMapSample.rgb, pin.NormalW, pin.TangentW);
    }
   
    PixelOut pout = (PixelOut) 0.0f;
    pout.WorldPosMatIndex = float4(pin.PosW, 1.0f);
    pout.NormalRoughness = float4(normalize(bumpedNormalW), gRoughness);
    pout.AlbedoAndMetallic = diffuseAlbedo;
    pout.ApecularAndEmissive = float4(gFresnelR0, 1.0f);
    return pout;
}