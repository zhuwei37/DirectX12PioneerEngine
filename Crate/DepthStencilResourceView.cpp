#include"DepthStencilResourceView.h"

//DepthStencilResourceView::DepthStencilResourceView(GraphicsDevice* GDevice,
//	GTexture resouce)
//{
//	GResource = std::make_unique<GTexture>(resouce);
//	//CreateHeapAndBindingResource(GDevice, this);
//}
 D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilResourceView::GetCPUView()const
{
	 return mDescriptor.CpuHandle;
}
 D3D12_GPU_DESCRIPTOR_HANDLE DepthStencilResourceView::GetGPUView()const
 {
	 return mDescriptor.GpuHandle;

 }
 std::shared_ptr<DepthStencilResourceView>DepthStencilResourceView::CreateHeapAndBindingResource(GraphicsDevice* GDevice, std::shared_ptr<GTexture> resource)
{
	 std::shared_ptr<DepthStencilResourceView> dsv=std::make_shared<DepthStencilResourceView>();
	
	dsv->mDescriptor= GDevice->DsvDescriptorHeapAllocator->Allocator(1);


	D3D12_DEPTH_STENCIL_VIEW_DESC desvDesc;
	desvDesc.Flags = D3D12_DSV_FLAG_NONE;
	desvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	desvDesc.Format = GDevice->DepthStencilFormat;
	desvDesc.Texture2D.MipSlice = 0;
	try
	{
		GDevice->mD3dDevice->CreateDepthStencilView(resource->NavigateResource(), &desvDesc, dsv->mDescriptor.CpuHandle);
	}
	catch (std::exception& e)
	{
		std::string err = e.what();
	}
	

	return dsv;
}
 std::shared_ptr<DepthStencilResourceView>DepthStencilResourceView::CreateViewAllotHeap(GraphicsDevice* GDevice)
 {
	 std::shared_ptr<DepthStencilResourceView> dsv = std::make_shared<DepthStencilResourceView>();
	 dsv->mDescriptor = GDevice->DsvDescriptorHeapAllocator->Allocator(1);
	 return dsv;
 }
 void DepthStencilResourceView::ResetBindHeap(GraphicsDevice *GDevice, std::shared_ptr<GTexture> resource)
 {
	 if (this->mDescriptor.Number > 0)
	 {
		 D3D12_DEPTH_STENCIL_VIEW_DESC desvDesc;
		 desvDesc.Flags = D3D12_DSV_FLAG_NONE;
		 desvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		 desvDesc.Format = GDevice->DepthStencilFormat;
		 desvDesc.Texture2D.MipSlice = 0;

		 GDevice->mD3dDevice->CreateDepthStencilView(resource->NavigateResource(), &desvDesc, this->mDescriptor.CpuHandle);

	 }
 }