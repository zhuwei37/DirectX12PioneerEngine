static const float2 gTexCoords[6] =
{
    float2(0.0f, 1.0f),
    float2(0.0f, 0.0f),
    float2(1.0f, 0.0f),
    float2(0.0f, 1.0f),
    float2(1.0f, 0.0f),
    float2(1.0f, 1.0f)
};


struct VertexOut
{
    float4 PosH : SV_Position;
   // float3 PosW : POSITION;
    float2 TexC : TEXCOORD;
};
VertexOut VSMain(uint vid : SV_VertexID)
{
    VertexOut vout;
    vout.TexC = gTexCoords[vid];
    //vout.PosW = float4(vout.TexC, 0, 1);
    //vout.PosW.x = vout.PosW.x * 2 - 1;
    //vout.PosW.y = vout.PosW.y * -2 + 1;
    vout.PosH = float4(2.0f * vout.TexC.x - 1.0f, 1.0f - 2.0f * vout.TexC.y, 0.0f, 1.0f);
    
    return vout;
}
