#include"Common.hlsl"

Texture2D gBaseColorMap : register(t0);
Texture2D gNormalMap : register(t1);
Texture2D gMetallicMap : register(t2);
Texture2D gRoughnessMap : register(t3);



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
    float3 baseColor = gBaseColorMap.Sample(gsamAnisotropicWrap, pin.TexC).rgb;
    float metallic = gMetallicMap.Sample(gsamAnisotropicWrap, pin.TexC).r;
    float roughness = gRoughnessMap.Sample(gsamAnisotropicWrap, pin.TexC).r;
    float4 normalMapSample = gNormalMap.Sample(gsamAnisotropicWrap, pin.TexC);
    
    float3 normalW = normalize(pin.NormalW);
    if ((normalMapSample.x + normalMapSample.y + normalMapSample.z) > 0.0f)
    {
        normalW = NormalSampleToWorldSpace(normalMapSample.rgb, pin.NormalW, pin.TangentW);

    }
    PixelOut pout;
    pout = (PixelOut) 0.0;
    pout.WorldPosMatIndex = float4(pin.PosW, 2);
    pout.NormalRoughness = float4(normalW, roughness);
    pout.AlbedoAndMetallic = float4(baseColor, metallic);
    
    return pout;

}