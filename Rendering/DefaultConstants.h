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