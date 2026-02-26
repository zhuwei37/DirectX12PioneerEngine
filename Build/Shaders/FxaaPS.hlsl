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
    float2 TexC : TEXCOORD;
};
float4 PSMain(PixelIn pIn) : SV_Target
{
    float4 m = gMap.Sample(gsamLinearWrap, pIn.TexC);
    float4 texPos = float4(pIn.TexC - gFxaaFrameInvSize.xy,pIn.TexC+gFxaaFrameInvSize.xy);
    float3 NE = gMap.Sample(gsamLinearWrap, texPos.zy);
    float3 NW = gMap.Sample(gsamLinearWrap, texPos.xy);
    float3 SE = gMap.Sample(gsamLinearWrap, texPos.zw);
    float3 SW = gMap.Sample(gsamLinearWrap, texPos.xw);
    
    
    float3 luma = float3(0.299, 0.587, 0.114);
    //提取亮度值
    float M_luma = dot(luma, m.rgb);
    float NE_luma = dot(luma, NE);
    float NW_luma = dot(luma, NW);
    float SE_luma = dot(luma, SE);
    float SW_luma = dot(luma, SW);
    
    //边缘检测
    float MaxLuma = max(max(NE_luma, NW_luma), max(SE_luma, SW_luma));
    float MinLuma = min(min(NE_luma, NW_luma), min(SE_luma, SW_luma));
    float Contrast = max(MaxLuma, M_luma) - min(MinLuma, M_luma);
    if (Contrast >= max(gFxaaConsoleEdgeMainThresholdMin, MaxLuma * gFxaaConsoleEdgeThreshold))
    {
        return m;
    }
    //计算法线
    float2 Dir1 = float2(0, 0);
    NE_luma += 1.0f / 384.0f;
    Dir1.x = -((NW_luma + NE_luma) - (SW_luma + SE_luma));
    Dir1.y = ((NE_luma + SE_luma) - (NW_luma + SW_luma));
    Dir1 = normalize(Dir1);
    
   
    //偏移
    float DirAbsMinTimesC = min(abs(Dir1.x), abs(Dir1.y)) * gFxaaConsoleEdgeSharpness;
     //计算切线
    float2 Dir2 = clamp(Dir1 / DirAbsMinTimesC, -2, 2) * 2;;
    //取值
    float N2 = gMap.Sample(gsamLinearWrap, Dir2 + pIn.TexC);
    //判读取值是否合理
    
    return float4(0, 0, 0, 0);

}