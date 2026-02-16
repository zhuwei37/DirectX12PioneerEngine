#pragma once


#include"d3dx12.h"
#include"d3dUtil.h"
#include"wrl.h"
using namespace Microsoft::WRL;
class SharedTexture
{
public :
	ID3D12Device* device;
	SharedTexture(ID3D12Device*,int width,int height);
	void CreateSharedTexture();
	void CreateSharedTextrueHandle();
	void OnResize(int width, int height);
	void CopyTexture(ID3D12Resource* input, ID3D12GraphicsCommandList* mCmdList);
	ID3D12Resource* GetResource()const
	{
		return mTexture.Get();
	}
	/*
	ID3D12Resource GetResource()const;*/
	HANDLE GetSharedTextureHandle();
private:

	ComPtr<ID3D12Resource> mTexture;
	int mWidth;
	int mHeight;
	HANDLE sharedHandle;

	
};