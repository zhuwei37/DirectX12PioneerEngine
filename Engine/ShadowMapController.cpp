#include"ShadowMapController.h"



ShadowMapController::ShadowMapController(GraphicsDevice* device)
{
	this->mGDevice = device;
	init();
}


void  ShadowMapController::OnRenderShadowMap(ID3D12GraphicsCommandList* mCommandList)
{



}
void ShadowMapController::BeginRenderShadowMap(ID3D12GraphicsCommandList* mCommandList)
{
	mCommandList->RSSetViewports(1, &mViewport);
	mCommandList->RSSetScissorRects(1, &mRect);
	CD3DX12_RESOURCE_BARRIER beforeResourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
		this->mShadowMapGraphicsResource->NavigateResource(), D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_DEPTH_WRITE
	);
	mCommandList->ResourceBarrier(1, &beforeResourceBarrier);
	mCommandList->ClearDepthStencilView(this->GetShadowMapDsvCPUHandle(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);
	auto handle = this->GetShadowMapDsvCPUHandle();
	mCommandList->OMSetRenderTargets(0, nullptr, true, &handle);


}

void ShadowMapController::EndRenderShadowMap(ID3D12GraphicsCommandList* mCommandList)
{
	CD3DX12_RESOURCE_BARRIER afterResourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
		this->mShadowMapGraphicsResource->NavigateResource()
		, D3D12_RESOURCE_STATE_DEPTH_WRITE, 
		D3D12_RESOURCE_STATE_GENERIC_READ
	);
	mCommandList->ResourceBarrier(1, &afterResourceBarrier);
}


void ShadowMapController::createDescriptor()
{
	mSrvDescriptor = this->mGDevice->CBV_SRV_UAV_Shader_Visible_DescriptorHeapAllocator->Allocator(1);
//	mDsvDecriptor = this->mGDevice->DsvDescriptorHeapAllocator->Allocator(1);
}
void ShadowMapController::createResource()
{
	ResourceDescription resourceDesc = { 0 };
	resourceDesc.ClearColor[0] = 0.0f;
	resourceDesc.ClearColor[1] = 0.0f;
	resourceDesc.ClearColor[2] = 0.0f;
	resourceDesc.ClearColor[3] = 0.0f;
	resourceDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	resourceDesc.Height = mShadowMapHeight;
	resourceDesc.Width = mShadowMapWidth;
	resourceDesc.ResourceState = D3D12_RESOURCE_STATE_GENERIC_READ;
	mShadowMapGraphicsResource=GTexture::CreateDepthStencilTexture(mGDevice, resourceDesc);
}

void ShadowMapController::init()
{
	createResource();
	createDescriptor();
	this->mSrv= ShaderResourceView::CreateShaderView(mGDevice, this->mShadowMapGraphicsResource.get(),mSrvDescriptor, DXGI_FORMAT_R24_UNORM_X8_TYPELESS);
	this->mDsv = DepthStencilResourceView::CreateHeapAndBindingResource(mGDevice, this->mShadowMapGraphicsResource);
	mViewport = { 0,0,static_cast<FLOAT>(mShadowMapWidth),static_cast<FLOAT>(mShadowMapHeight),0,1.0f };
	mRect = { 0,0,mShadowMapWidth,mShadowMapHeight };

}
D3D12_CPU_DESCRIPTOR_HANDLE ShadowMapController::GetShadowMapDsvCPUHandle()
{
	return this->mDsv->GetCPUView();
}
D3D12_GPU_DESCRIPTOR_HANDLE ShadowMapController::GetShadowMapSrvGPUHandle()
{
	return this->mSrv->GetGPUView();
}