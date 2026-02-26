#pragma once
#include"GraphicsDevice.h"
#include"MaterialContext.h"
#include"RenderTargetView.h"
#include"ShaderResourceView.h"
#include"GTexture.h"
#include"Descriptor.h"
class FxaaController
{
	
public:
	FxaaController(GraphicsDevice *device,UINT width,UINT height);
	void PreRender(ID3D12GraphicsCommandList* mCommandList);
	void OnRender(ID3D12GraphicsCommandList* mCommandList,D3D12_GPU_DESCRIPTOR_HANDLE useSrvMap);
	void EndRender(ID3D12GraphicsCommandList* mCommandList);
	void Resize(UINT height, UINT width);
	D3D12_GPU_DESCRIPTOR_HANDLE GetPostProcessedMap();
private:
	UINT mHeight;
	UINT mWidth;
	void Init();
	void CreateResource();
	void BuildDescriptor();
	std::shared_ptr<MaterialContext> mFaxxMaterialContext;
	std::shared_ptr<RenderTargetView> mRtv;
	std::shared_ptr<ShaderResourceView> mSrv;
	std::shared_ptr<GTexture> mFxaaTexture;
	Descriptor mRtvDescriptor;
	Descriptor mSrvDescritpor;
	GraphicsDevice* mGDevice;
	DXGI_FORMAT mFxaaFormat = DXGI_FORMAT_R8G8B8A8_UNORM;;


	D3D12_VIEWPORT mViewPort;
	D3D12_RECT mRect;
};