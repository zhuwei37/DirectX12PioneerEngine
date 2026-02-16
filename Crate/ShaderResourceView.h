#pragma once
#include"ResourceView.h"
#include"Descriptor.h"
class ShaderResourceView :ResourceView
{
public:
	ShaderResourceView() {};
	ShaderResourceView(Descriptor Descriptor) 
	{
		this->mDescriptor = Descriptor;
	}
	virtual D3D12_CPU_DESCRIPTOR_HANDLE GetCPUView()const override;
	virtual D3D12_GPU_DESCRIPTOR_HANDLE GetGPUView()const override;
	static std::shared_ptr<ShaderResourceView> CreateShaderView(GraphicsDevice *GDevice,
		GraphicsResource* viewResource, D3D12_SRV_DIMENSION DIMENSION= D3D12_SRV_DIMENSION_TEXTURE2D
	);
	static std::shared_ptr< ShaderResourceView> CreateShaderView(
		GraphicsDevice* GDevice,
		GraphicsResource* viewResource, Descriptor d,DXGI_FORMAT format, D3D12_SRV_DIMENSION DIMENSION = D3D12_SRV_DIMENSION_TEXTURE2D);
private:

	Descriptor mDescriptor;
};