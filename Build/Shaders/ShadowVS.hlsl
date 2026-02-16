#include"Common.hlsl"
struct VertexIn
{
    float3 PosL : POSITION;
    float2 TexC : TEXCOORD;
};
struct VertexOut
{
    float4 PosH : SV_Position;
    float2 TexC : TEXCOORD;
};


VertexOut VSMain(VertexIn vin)
{
    VertexOut vout = (VertexOut) 0;
    
    float4 posW = mul(float4(vin.PosL, 1.0f), gWorld);
    vout.PosH = mul(posW, gViewProj);
    vout.TexC = vin.TexC;
    return vout;

}