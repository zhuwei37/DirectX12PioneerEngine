#include"RenderHost.h"
#include"GTexture.h"
#include"ActorManager.h"
#include<DirectXMath.h>

RenderHost::RenderHost
(GraphicsDevice* GDevice, CreateRenderHostParameter* paramter)
	:
	mGDevice(GDevice)
	//阴影渲染PASS
	,mCommonPassConstantsRenderData(GDevice,1,true),
	mCommonLightRenderData(GDevice,16,true),
	mCommonObjectPassContantsRenderData(GDevice,4096,true)
{
	mWidth = paramter->Width;
	mHeight = paramter->Height;
	mPresenter = std::make_shared<SwapChainGraphicsPresenter>(mGDevice,(HWND) paramter->WindowHandle, mWidth, mHeight);
	initialization();
	//mSkyBox = std::make_shared<SkyBox>();
}
bool RenderHost::IsVisible()
{
	return mIsVisible;
}
void RenderHost::SetVisible(bool isVis)
{
	mIsVisible = isVis;
}
CameraController* RenderHost::GetCameraController()
{
	return mCamera.get();
}
void RenderHost::initialization()
{
	mCamera = std::make_shared<CameraController>();
	mCamera->SetPosition(0.0f, 2.0f, -15.0);
	mCamera->Resize(mWidth, mHeight);
	ResourceDescription resc{ mWidth,mHeight };
	resc.ResourceState = D3D12_RESOURCE_STATE_DEPTH_WRITE;
	resc.Format = mGDevice->DepthStencilFormat;
	mDepthStencilGraphicsResource = GTexture::CreateDepthStencilTexture(mGDevice, resc);
	mDsv= DepthStencilResourceView::CreateHeapAndBindingResource(mGDevice, mDepthStencilGraphicsResource);

	
	mViewPort.TopLeftX = 0;
	mViewPort.TopLeftY = 0;
	mViewPort.Width = static_cast<float>(mWidth);
	mViewPort.Height = static_cast<float>(mHeight);
	mViewPort.MinDepth = 0.0f;
	mViewPort.MaxDepth = 1.0f;

	mRect = { 0,0,mWidth,mHeight };

	ThrowGraphicsfFailed( mGDevice->mD3dDevice->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(mCommandAllocator.GetAddressOf())
	));
	ThrowGraphicsfFailed(mGDevice->mD3dDevice->CreateCommandList
	(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		mCommandAllocator.Get(),
		nullptr,
		IID_PPV_ARGS(mCommandList.GetAddressOf())
	));
	mCommandList->Close();


}/*
void RenderHost::initSky()
{
	this->mSkyActor = ActorManager::Get()->CreateActor();
}*/
void RenderHost::PreRender(std::map<Layer, RendererList> &renderers, SkyBox* skybox)
{
	
	mActivityRenderers.clear();

	this->mSkyBox = skybox;
	//进行视锥剔除
	for (auto& renderer : renderers[Default])
	{
		mActivityRenderers.push_back(renderer);
	}


	//更新光照
	UpdateLight();
	//摄像机矩阵
	UpdateCommonPassConstants();
	//更新渲染项
	UpdateObjectPassConstants();
}
void RenderHost::UpdateObjectPassConstants()
{
	objIndex = 0;
	for (auto renderer : mActivityRenderers)
	{
		ObjectPassConstants mObjectPass;
		auto transform = renderer->GetTransformMatrix();
		renderer->SetObjectPassContantIndex(objIndex);
		DirectX::XMMATRIX transpose;
		transpose=DirectX::XMLoadFloat4x4(&transform);
		transpose=DirectX::XMMatrixTranspose(transpose);
		DirectX::XMStoreFloat4x4(&transform,transpose);
		mObjectPass.World = transform;
		
		mCommonObjectPassContantsRenderData.SetData(objIndex, mObjectPass);
		objIndex++;
	}
	
	this->mSkyObjIndex = this->objIndex;
	if (this->mSkyBox != nullptr)
	{
		ObjectPassConstants mSkyObjectPass;
		auto transform = this->mSkyBox->Update();
		auto m = transform->GetWorldMatrix();
		auto transpoes = DirectX::XMLoadFloat4x4(&m);
		auto transpose = DirectX::XMMatrixTranspose(transpoes);
		DirectX::XMStoreFloat4x4(&mSkyObjectPass.World, transpose);
		mCommonObjectPassContantsRenderData.SetData(this->mSkyObjIndex, mSkyObjectPass);
	}
	this->objIndex = this->mSkyObjIndex + 1;
}
void RenderHost::UpdateLight()
{
	this->mCommonLight.Direction = { 0.57735f, -0.57735f, 0.57735f };
	this->mCommonLight.Strength = { 0.8f, 0.8f, 0.8f };
	this->mCommonLightRenderData.SetData(0, mCommonLight);
}
void RenderHost::UpdateCommonPassConstants()
{
	mCamera->UpdateViewMatrix();
	DirectX::XMMATRIX view = mCamera->GetView();
	DirectX::XMMATRIX proj = mCamera->GetProj();
	DirectX::XMFLOAT3 pos = mCamera->GetPosition3f();

	DirectX::XMMATRIX viewProj = XMMatrixMultiply(view, proj);
	auto d = XMMatrixDeterminant(view);
	DirectX::XMMATRIX invView = XMMatrixInverse(&d, view);
	d = XMMatrixDeterminant(proj);
	DirectX::XMMATRIX invProj = XMMatrixInverse(&d, proj);
	d = XMMatrixDeterminant(viewProj);
	DirectX::XMMATRIX invViewProj = XMMatrixInverse(&d, viewProj);

	XMStoreFloat4x4(&this->mCommonPass.View, XMMatrixTranspose(view));
	XMStoreFloat4x4(&mCommonPass.InvView, XMMatrixTranspose(invView));
	XMStoreFloat4x4(&mCommonPass.Proj, XMMatrixTranspose(proj));
	XMStoreFloat4x4(&mCommonPass.InvProj, XMMatrixTranspose(invProj));
	XMStoreFloat4x4(&mCommonPass.ViewProj, XMMatrixTranspose(viewProj));
	XMStoreFloat4x4(&mCommonPass.InvViewProj, XMMatrixTranspose(invViewProj));

	mCommonPass.EyePosW = pos;
	mCommonPass.AmbientLight = { 0.25f,0.25f,0.35f,1.0f };
	mCommonPassConstantsRenderData.SetData(0, mCommonPass);
}
void RenderHost::AddRenderer(Renderer* renderer)
{
	mRenderers.push_back(renderer);
}
void RenderHost::RemoveRenderer(Renderer* renderer)
{
	auto removeRenderer=std::remove(mRenderers.begin(), mRenderers.end(), renderer);
	if (removeRenderer!=mRenderers.end())
	{
		mRenderers.erase(removeRenderer);
	}
}
ComPtr<ID3D12GraphicsCommandList> RenderHost::OnRender()
{
	auto commandList = this->mCommandList;
	
	//重置命令列表
	this->mCommandAllocator->Reset();
	this->mCommandList->Reset(this->mCommandAllocator.Get(), nullptr);


	ID3D12DescriptorHeap* descriptorHeaps[] = { mGDevice->CBV_SRV_UAV_Shader_Visible_DescriptorHeapAllocator->GetNavigateDescriptorHeap() };
	
	//设置视口
	commandList->RSSetViewports(1, &mViewPort);
	commandList->RSSetScissorRects(1, &mRect);
	// 设置RTV、DSV
	//设置资源屏障
	D3D12_RESOURCE_BARRIER rtvResourceBar = CD3DX12_RESOURCE_BARRIER::Transition(mPresenter->GetCurrentBuffer(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	commandList->ResourceBarrier(1, &rtvResourceBar);
	commandList->ClearRenderTargetView(mPresenter->GetCurrentBufferView(), DirectX::Colors::LightBlue, 0, nullptr);
	commandList->ClearDepthStencilView(mDsv->GetCPUView(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);
	auto curBackBufferAddr = mPresenter->GetCurrentBufferView();
	auto curDsvAddr = mDsv->GetCPUView();
	commandList->OMSetRenderTargets(1, &curBackBufferAddr, true, &curDsvAddr);
	commandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);
	//绑定通用参数
	//commandList->SetGraphicsRootConstantBufferView(0,)

	//执行渲染器 
	//优化存在渲染器活动列表不用进行判断
	for (auto renderer : mActivityRenderers)
	{
		renderer->OnRender(commandList, this);
		
	}
	//设置资源屏障
	D3D12_RESOURCE_BARRIER rtvResourceBar2 = CD3DX12_RESOURCE_BARRIER::Transition(mPresenter->GetCurrentBuffer(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	commandList->ResourceBarrier(1, &rtvResourceBar2);
	//渲染视图翻转

	//mGDevice->ExctureCommands();

	//mPresenter->Present();
	//等待
	ThrowGraphicsfFailed(commandList->Close());
	//结束
	return commandList;
}
void RenderHost::Present()
{
	mPresenter->Present();
}
//void RenderHost::OnRender(ComPtr<ID3D12GraphicsCommandList> commandList)
//{
//	//设置视口
//	commandList->RSSetViewports(1, &mViewPort);
//	commandList->RSSetScissorRects(1, &mRect);
//	// 设置RTV、DSV
//	//设置资源屏障
//	D3D12_RESOURCE_BARRIER rtvResourceBar = CD3DX12_RESOURCE_BARRIER::Transition(mPresenter->GetCurrentBuffer(),
//		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
//	commandList->ResourceBarrier(1, &rtvResourceBar);
//	commandList->ClearRenderTargetView(mPresenter->GetCurrentBufferView(), DirectX::Colors::LightBlue, 0, nullptr);
//	commandList->ClearDepthStencilView(mDsv->GetCPUView(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);
//	auto curBackBufferAddr = mPresenter->GetCurrentBufferView();
//	auto curDsvAddr = mDsv->GetCPUView();
//	commandList->OMSetRenderTargets(1, &curBackBufferAddr, true, &curDsvAddr);
//	//绑定通用参数
//	//commandList->SetGraphicsRootConstantBufferView(0,)
//	
//	//执行渲染器 
//	//优化存在渲染器活动列表不用进行判断
//	for (auto renderWeakPtr : mRenderers)
//	{
//		auto render = renderWeakPtr.lock();
//		if (render->GetIsEnable())
//		{
//			render->OnRender(commandList,this);
//		}
//	}
//	//设置资源屏障
//	D3D12_RESOURCE_BARRIER rtvResourceBar2 = CD3DX12_RESOURCE_BARRIER::Transition(mPresenter->GetCurrentBuffer(),
//		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
//	commandList->ResourceBarrier(1, &rtvResourceBar2);
//	//渲染视图翻转
//	
//	mGDevice->ExctureCommands();
//
//	mPresenter->Present();
//	//等待
//
//	//结束
//
//}
void RenderHost::Resize(int width, int height)
{
	if (width <= 0 || height <= 0)
	{
		return;
	}
	

	mWidth = width;
	mHeight = height;
	mCamera->Resize(mWidth, mHeight);
	mPresenter->Resize(mWidth, mHeight);
	ResourceDescription rd = { mWidth,mHeight };
	rd.ResourceState = D3D12_RESOURCE_STATE_DEPTH_WRITE;
	rd.Format = mGDevice->DepthStencilFormat;
	mDepthStencilGraphicsResource = GTexture::CreateDepthStencilTexture(mGDevice, rd);
	mDsv->ResetBindHeap(mGDevice, mDepthStencilGraphicsResource);
	



	mViewPort.TopLeftX = 0;
	mViewPort.TopLeftY = 0;
	mViewPort.Width = static_cast<float>(mWidth);
	mViewPort.Height = static_cast<float>(mHeight);
	mViewPort.MinDepth = 0.0f;
	mViewPort.MaxDepth = 1.0f;

	mRect = { 0,0,mWidth,mHeight };
}