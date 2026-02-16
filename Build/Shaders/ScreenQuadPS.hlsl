#include"Common.hlsl"
Texture2D textureMap : register(t0);

struct VertexOut
{
    float4 PosH : SV_POSITION;
    float2 TexC : TEXCOORD;
};



float4 PSMain(VertexOut pin):SV_Target
{
    float4 color = textureMap.Sample(gsamLinearWrap, pin.TexC);
    return float4(color.rgb,1.0f);

}



