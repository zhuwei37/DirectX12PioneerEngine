#pragma once
#include"IComponent.h"
#include"DirectXMath.h"
#include<wrl.h>
#include"d3d12.h"
#include"interface.h"
class RenderHost;
class Renderer:public IComponent
{
public:
	virtual void PreRender() = 0;
	virtual void OnRender(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList,RenderHost* renderHost)=0;
	void SetObjectPassContantIndex(int index)
	{
		mObjectPassConstantIndex = index;
	}
	int GetObjectPassContantIndex()
	{
		return mObjectPassConstantIndex;
	}
	virtual DirectX::XMFLOAT4X4 GetTransformMatrix()=0;
protected:
	int mObjectPassConstantIndex;
	//virtual DirectX::XMFLOAT4X4 GetTransform()=0;
	
};