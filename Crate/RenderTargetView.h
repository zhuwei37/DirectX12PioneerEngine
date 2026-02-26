#pragma once
#include"ResourceView.h"
#include"Descriptor.h"
#include"GTexture.h"
class RenderTargetView :ResourceView
{
public:
	RenderTargetView() {};
	virtual D3D12_CPU_DESCRIPTOR_HANDLE GetCPUView()const override;
	virtual D3D12_GPU_DESCRIPTOR_HANDLE GetGPUView()const override;

	static std::shared_ptr<RenderTargetView> CreateRenderTargetView
	(
		GraphicsDevice* mGDevice,
		GTexture* texture, Descriptor d, DXGI_FORMAT format
	);
private:
	Descriptor mDescriptor;
};