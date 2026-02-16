//***************************************************************************************
// d3dApp.cpp by Frank Luna (C) 2015 All Rights Reserved.
//***************************************************************************************

#include "d3dApp.h"
#include <WindowsX.h>
#include"Device.h"
using Microsoft::WRL::ComPtr;
using namespace std;
using namespace DirectX;


D3DApp::D3DApp()

{

}

D3DApp::~D3DApp()
{
	if (md3dDevice != nullptr)
		FlushCommandQueue();
}






bool D3DApp::Initialize()
{

	if (!InitDirect3D())
		return false;
	OnResize(mClientWidth, mClientHeight);

	return true;
}


void D3DApp::OnResize(int width, int height)
{
	assert(md3dDevice);

	// Flush before changing any resources.
	FlushCommandQueue();

	//ThrowGraphicsfFailed(mCommandList->Reset(mDirectCmdListAlloc.Get(), nullptr));
	mDevice->ResetDirectCommandList();

	Presenter->Resize(mClientWidth, mClientHeight);

	ResourceDescription resDesc{mClientWidth,mClientHeight};

	auto DsvResource=GTexture::CreateDepthStencilTexture(mDevice.get(), resDesc);

	 mDsv->ResetBindHeap(mDevice.get(), DsvResource);
	//mDsv = std::make_unique<DepthStencilResourceView>(dsv);
	// Transition the resource from its initial state to be used as a depth buffer.
	//mDevice->mDirectCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mDsv->GResource->Resource.Get(),
	//	D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE));

	// Execute the resize commands.
	/*ThrowGraphicsfFailed(mCommandList->Close());
	ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };*/
	//mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);
	/*mDevice->DirectCommandQueue->Execute(cmdsLists);*/
	mDevice->ExctureCommands();
	// Wait until resize is complete.
	FlushCommandQueue();

	// Update the viewport transform to cover the client area.
	mScreenViewport.TopLeftX = 0;
	mScreenViewport.TopLeftY = 0;
	mScreenViewport.Width = static_cast<float>(mClientWidth);
	mScreenViewport.Height = static_cast<float>(mClientHeight);
	mScreenViewport.MinDepth = 0.0f;
	mScreenViewport.MaxDepth = 1.0f;

	mScissorRect = { 0, 0, mClientWidth, mClientHeight };
}
bool D3DApp::InitDirect3D()
{
	

	this->md3dDevice = Device::Get()->mDX12Device;
	this->mDevice = std::make_unique<GraphicsDevice>();
	this->Presenter = std::make_unique<SwapChainGraphicsPresenter>(this->mDevice.get(), this->mHandle, this->mClientWidth, this->mClientHeight);
	mDsv = DepthStencilResourceView::CreateViewAllotHeap(mDevice.get());
	

	return true;
}




void D3DApp::FlushCommandQueue()
{
	mDevice->DirectCommandQueue->FlushCommandQueue();
}
