#pragma once
#include"GraphicsDevice.h"
#include"GTexture.h"
#include"Descriptor.h"
#include"ShaderResourceView.h"
#include"DepthStencilResourceView.h"
class ShadowMapController
{
public:
	ID3D12Resource* NavigateResource()
	{
		return mShadowMapGraphicsResource->NavigateResource();
	}
public :
	ShadowMapController( GraphicsDevice* device);
	void BeginRenderShadowMap(ID3D12GraphicsCommandList* mCommandList);

	void EndRenderShadowMap(ID3D12GraphicsCommandList* mCommandList);
	void OnRenderShadowMap(ID3D12GraphicsCommandList* mCommandList);
public:
	D3D12_CPU_DESCRIPTOR_HANDLE GetShadowMapDsvCPUHandle();
	D3D12_GPU_DESCRIPTOR_HANDLE GetShadowMapSrvGPUHandle();
private:
	void init();
	void createResource();
	void createDescriptor();
	int mShadowMapWidth=2048;
	int mShadowMapHeight=2048;
	std::shared_ptr<ShaderResourceView> mSrv;
	std::shared_ptr<DepthStencilResourceView> mDsv;
	std::shared_ptr<GTexture> mShadowMapGraphicsResource;
	Descriptor mSrvDescriptor;
//	Descriptor mDsvDecriptor;
	GraphicsDevice* mGDevice;


	D3D12_VIEWPORT mViewport;
	D3D12_RECT mRect;

};