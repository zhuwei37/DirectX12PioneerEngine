#include"Common.hlsl"


cbuffer cbParameter : register(b3)
{
    float3 gAlbedo;
    float gRoughness;
    float gMetalness;

}

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
    PixelOut pout;
    pout.WorldPosMatIndex = float4(pin.PosW, 4);
    pout.NormalRoughness = float4(pin.NormalW, gRoughness);
    pout.AlbedoAndMetallic = float4(gAlbedo, gMetalness);
    pout.ApecularAndEmissive = float4(0.0, 0.0, 0.0, 0.0);
    return pout;
}
