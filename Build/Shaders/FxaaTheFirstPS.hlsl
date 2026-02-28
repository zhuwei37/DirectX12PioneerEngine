//Fxaa 初代目

cbuffer cbFxaa : register(b0)
{
    //渲染尺寸的倒数
    float4 gFxaaFrameInvSize;
    //锐度
    float gFxaaConsoleEdgeSharpness;
    float gFxaaConsoleEdgeThreshold;
    float gFxaaConsoleEdgeMainThresholdMin;
}

Texture2D gMap : register(t0);

SamplerState gsamPointWrap : register(s0);
SamplerState gsamPointClamp : register(s1);
SamplerState gsamLinearWrap : register(s2);
SamplerState gsamLinearClamp : register(s3);
SamplerState gsamAnisotropicWrap : register(s4);
SamplerState gsamAnisotropicClamp : register(s5);

struct PixelIn
{
    float4 PosH : SV_Position;
    float3 PosW : POSITION;
    float2 TexC : TEXCOORD;
};
float4 PSMain(PixelIn pIn) : SV_Target
{
    
        

}