#pragma once
#include"ResourceView.h"
#include"GTexture.h"
class DepthStencilResourceView:public ResourceView 
{
public:
	DepthStencilResourceView() {};
	//DepthStencilResourceView(GraphicsDevice* GDevice, GTexture resource);
	
	virtual D3D12_CPU_DESCRIPTOR_HANDLE GetCPUView()const override  ;
	virtual D3D12_GPU_DESCRIPTOR_HANDLE GetGPUView()const override;
	static std::shared_ptr<DepthStencilResourceView> CreateHeapAndBindingResource(GraphicsDevice* GDevice,std::shared_ptr<GTexture> resource);
	static std::shared_ptr<DepthStencilResourceView> CreateViewAllotHeap(GraphicsDevice* GDevice);

public:
	void ResetBindHeap(GraphicsDevice* ,std::shared_ptr<GTexture> resource);
private:
	Descriptor mDescriptor;
};