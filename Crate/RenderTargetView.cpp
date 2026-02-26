#include"RenderTargetView.h"


D3D12_CPU_DESCRIPTOR_HANDLE RenderTargetView::GetCPUView()const
{
	return this->mDescriptor.CpuHandle;
}
D3D12_GPU_DESCRIPTOR_HANDLE RenderTargetView::GetGPUView()const
{
	return this->mDescriptor.GpuHandle;
}
std::shared_ptr<RenderTargetView> RenderTargetView::CreateRenderTargetView(GraphicsDevice* mGDevice,
	GTexture* texture, Descriptor d, DXGI_FORMAT format)
{
	std::shared_ptr<RenderTargetView> rtv = std::make_shared<RenderTargetView>();
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = format;
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;
	rtvDesc.Texture2D.PlaneSlice = 0;
	mGDevice->mD3dDevice->CreateRenderTargetView(texture->NavigateResource(), &rtvDesc, d.CpuHandle);
	rtv->mDescriptor = d;
	return rtv;
}