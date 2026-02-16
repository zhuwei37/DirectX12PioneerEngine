#include"Common.hlsl"

cbuffer cbMaterial : register(b3)
{
    float4 gColor;
}

struct VertexIn
{
    float3 PosL : POSITION;
    float3 NormalL : NORMAL;
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


PixelOut PS(VertexOut pin)
{
    float4 diffuseAlbedo = gColor ;
    PixelOut pout = (PixelOut) 0;
    pout.WorldPosMatIndex = float4(pin.PosW, 3);
    pout.NormalRoughness = float4(pin.NormalW, 0.0);
    pout.AlbedoAndMetallic = diffuseAlbedo;
    return pout;

}