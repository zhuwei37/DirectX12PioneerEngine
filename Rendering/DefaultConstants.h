#pragma once
#include<DirectXMath.h>
#include"GMathHelper.h"
struct CommonPassConstants
{
    DirectX::XMFLOAT4X4 View = GMathHelper::Identity4x4();
    DirectX::XMFLOAT4X4 InvView = GMathHelper::Identity4x4();
    DirectX::XMFLOAT4X4 Proj = GMathHelper::Identity4x4();
    DirectX::XMFLOAT4X4 InvProj = GMathHelper::Identity4x4();
    DirectX::XMFLOAT4X4 ViewProj = GMathHelper::Identity4x4();
    DirectX::XMFLOAT4X4 InvViewProj = GMathHelper::Identity4x4();
    DirectX::XMFLOAT4X4 ShadowTransform = GMathHelper::Identity4x4();
    DirectX::XMFLOAT3 EyePosW = { 0.0f, 0.0f, 0.0f };
    float cbPerObjectPad1 = 0.0f;
    DirectX::XMFLOAT2 RenderTargetSize = { 0.0f, 0.0f };
    DirectX::XMFLOAT2 InvRenderTargetSize = { 0.0f, 0.0f };
    float NearZ = 0.0f;
    float FarZ = 0.0f;
    float TotalTime = 0.0f;
    float DeltaTime = 0.0f;
    DirectX::XMFLOAT4 AmbientLight = { 0.25f, 0.25f, 0.35f, 1.0f};
};
//UINT CalcConstantBufferByteSize(UINT byteSize)
//{
//    return (byteSize + 255) & ~255;
//}
typedef  struct ObjectPassConstants
{
    DirectX::XMFLOAT4X4 World;
}ObjectPassConstants;


typedef struct SsaoConstants
{
    DirectX::XMFLOAT4X4 Proj;
    DirectX::XMFLOAT4X4 InvProj;
    DirectX::XMFLOAT4X4 ProjTex;
    DirectX::XMFLOAT4 OffsetVectors[14];

    DirectX::XMFLOAT4 BlurWeights[3];

    DirectX::XMFLOAT2 InvRenderTargetSize = { 0.0f,0.0f };

    float OcclusionRadius = 0.5f;
    float OcclusionFadeStart = 0.2f;
    float OcclusionFadeEnd = 2.0f;
    float SurfaceEpsilon = 0.05f;
}SsaoConstants;

typedef struct FxaaConstants
{
    /// <summary>
    /// Œ∆¿Ì≥ﬂ¥ÁƒÊ
    /// </summary>
    DirectX::XMFLOAT4 gFxaaFrameInvSize;
    /// <summary>
    /// ±ﬂ‘µ»Ò∂»
    /// </summary>
    float gFxaaConsoleEdgeSharpness;
    /// <summary>
    /// ±ﬂ‘µ„–÷µ
    /// </summary>
    float gFxaaConsoleEdgeThreshold;
    /// <summary>
    /// ◊Ó–°±ﬂ‘µ„–÷µ
    /// </summary>
    float gFxaaConsoleEdgeMainThresholdMin;
};
