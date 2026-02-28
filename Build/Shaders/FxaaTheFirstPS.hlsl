//Fxaa 初代目

cbuffer cbFxaa : register(b0)
{
    //渲染尺寸的倒数 x:1/width y:1/height
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
float4 SampleRGBLuma(float2 tex)
{
    float4 color = gMap.Sample(gsamLinearWrap, tex);
    float3 lumaScaler = float3(0.299, 0.587, 0.114);
    float luma = dot(color.rgb, lumaScaler);
    return float4(color.rgb,luma);

}

float4 PSMain(PixelIn pin) : SV_Target
{
    //采集周围亮度
    float4 N_rgbl = SampleRGBLuma(pin.TexC + float2(0, -gFxaaFrameInvSize.y));//北
    float4 S_rgbl = SampleRGBLuma(pin.TexC + float2(0, gFxaaFrameInvSize.y));//南
    float4 E_rgbl = SampleRGBLuma(pin.TexC + float2(gFxaaFrameInvSize.x, 0));//东
    float4 W_rgbl = SampleRGBLuma(pin.TexC + float2(-gFxaaFrameInvSize.x, 0));//西
    float4 M_rgbl = SampleRGBLuma(pin.TexC);//中间
    
    float LumaN = N_rgbl.w;
    float LumaS = S_rgbl.w;
    float LumaE = E_rgbl.w;
    float LumaW = W_rgbl.w;
    float LumaM = M_rgbl.w;
    
    float MaxLuma = max(LumaM, max(max(LumaN, LumaS), max(LumaE, LumaW)));
    float MinLuma = min(LumaM, min(min(LumaN, LumaS), min(LumaE, LumaW)));
    
    float LumaContrast = MaxLuma - MinLuma;
    
    //检测是否是边缘
    if (LumaContrast   <= max(gFxaaConsoleEdgeMainThresholdMin, MaxLuma * gFxaaConsoleEdgeThreshold))
    {
        return float4(M_rgbl.rgb, 1.0f);
    }
    
    
    float LumaGradS = LumaS - LumaM;
    float LumaGradN = LumaN - LumaM;
    float LumaGradW = LumaW - LumaM;
    float LumaGradE = LumaE - LumaM;
    
    //判断梯度方向
    float LumaGradV = abs(LumaGradS + LumaGradN);
    float LumaGradH = abs(LumaGradW + LumaGradE);
    bool isHorz = LumaGradV > LumaGradH;
    //判断法线方向
    
    float2 normal = float2(0.0f, 0.0f);
    //if (isHorz)
    //{
    //    normal.y = sign(abs(LumaGradN - abs(LumaGradS)));
    //}
    //else
    //{
    //    normal.x = sign(abs(LumaGradE) - abs(LumaGradW));
    //}
    if (isHorz)
    {
        normal.x = sign((LumaGradN - (LumaGradS)));
    }
    else
    {
        normal.y = sign((LumaGradE) - (LumaGradW));
    }
    
    float LumaL = (LumaN + LumaS + LumaE + LumaW) * 0.25;
    float LumaDeltaML = abs(LumaM - LumaL);
    float blendL = LumaDeltaML / LumaContrast;
    
    float4 color1 = gMap.Sample(gsamLinearWrap, pin.TexC + normal * blendL);
    float4 color2 = gMap.Sample(gsamLinearWrap, pin.TexC - normal * blendL);
    return lerp(color1,color2,0.5);

}