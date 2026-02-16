#include"ShaderResourceView.h"
#include"GraphicsDevice.h"
D3D12_CPU_DESCRIPTOR_HANDLE  ShaderResourceView::GetCPUView()const
{
	return mDescriptor.CpuHandle;
}
D3D12_GPU_DESCRIPTOR_HANDLE ShaderResourceView::GetGPUView()const
{
	return mDescriptor.GpuHandle;
}
std::shared_ptr< ShaderResourceView> ShaderResourceView::CreateShaderView(
	GraphicsDevice* GDevice,
 GraphicsResource* viewResource, D3D12_SRV_DIMENSION DIMENSION 
)
{
	std::shared_ptr<ShaderResourceView> srv = std::make_shared<ShaderResourceView>();
	auto d=  GDevice->CBV_SRV_UAV_No_Shader_Visible_DescriptorHeapAllocator->Allocator(1);
	srv->mDescriptor = d;
	//std::unique_ptr< GraphicsResource> gr = std::unique_ptr<GraphicsResource>();
	//gr->Resource = uploadResource;
	//srv->GResource = std::move(gr);
	
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = viewResource->NavigateResource()->GetDesc().Format;
	if (DIMENSION == D3D12_SRV_DIMENSION_TEXTURE2D)
	{

		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = viewResource->NavigateResource()->GetDesc().MipLevels;
		srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
	}
	else if (DIMENSION == D3D12_SRV_DIMENSION_TEXTURECUBE)
	{
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
		srvDesc.TextureCube.MostDetailedMip = 0;
		srvDesc.TextureCube.MipLevels = viewResource->NavigateResource()->GetDesc().MipLevels;
		srvDesc.TextureCube.ResourceMinLODClamp = 0.0f;
	}
	GDevice->mD3dDevice->CreateShaderResourceView(viewResource->NavigateResource(), &srvDesc, srv->GetCPUView());
	return srv;
}
std::shared_ptr< ShaderResourceView> ShaderResourceView::CreateShaderView(
	GraphicsDevice* GDevice,
	GraphicsResource* viewResource, Descriptor d, DXGI_FORMAT format,D3D12_SRV_DIMENSION DIMENSION)
{
	std::shared_ptr<ShaderResourceView> srv = std::make_shared<ShaderResourceView>();
	srv->mDescriptor = d;
	//std::unique_ptr< GraphicsResource> gr = std::unique_ptr<GraphicsResource>();
	//gr->Resource = uploadResource;
	//srv->GResource = std::move(gr);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = format;
	if (DIMENSION == D3D12_SRV_DIMENSION_TEXTURE2D)
	{

		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = viewResource->NavigateResource()->GetDesc().MipLevels;
		srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
	}
	else if (DIMENSION == D3D12_SRV_DIMENSION_TEXTURECUBE)
	{
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
		srvDesc.TextureCube.MostDetailedMip = 0;
		srvDesc.TextureCube.MipLevels = viewResource->NavigateResource()->GetDesc().MipLevels;
		srvDesc.TextureCube.ResourceMinLODClamp = 0.0f;
	}
	GDevice->mD3dDevice->CreateShaderResourceView(viewResource->NavigateResource(), &srvDesc, srv->GetCPUView());
	return srv;
}