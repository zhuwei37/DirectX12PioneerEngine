#include"Common.hlsl"


TextureCube gCubeMap : register(t0);

struct VertexIn
{
    float3 PosL : POSITION;
    float3 NormalL : NORMAL;
    float2 TexC : TEXCOORD;
};
struct VertexOut
{
    float4 PosH : SV_POSITION;
    float3 PosL : POSITION;
};
VertexOut VS(VertexIn vin)
{
    VertexOut vout;

	// Use local vertex position as cubemap lookup vector.
    vout.PosL = vin.PosL;
	
	// Transform to world space.
    float4 posW = mul(float4(vin.PosL, 1.0f), gWorld);
   //  posW = float4(vin.PosL.x * 5000, vin.PosL.y * 5000, vin.PosL.z * 5000, 1.0f);
	// Always center sky about camera.
    posW.xyz += gEyePosW;

	// Set z = w so that z/w = 1 (i.e., skydome always on far plane).
    vout.PosH = mul(posW, gViewProj).xyww;
	
    return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
  //  return  float4(0.2f, 0.5f, 0.8f,1.0f);
    return gCubeMap.Sample(gsamLinearWrap, (pin.PosL));
}
