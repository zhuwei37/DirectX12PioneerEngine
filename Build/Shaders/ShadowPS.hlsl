struct PixelIn
{
    float4 PosH : SV_Position;
    float2 TexC : TEXCOORD;
};


void PSMain(PixelIn pin)
{
    float4 a = pin.PosH;
}