#include"FxaaController.h"
#include"MaterialContexts.h"
FxaaController::FxaaController(GraphicsDevice* device, int width, int height):
	mGDevice(device),
	mWidth(width),
	mHeight(height),
	mFxaaCB(device,1,false)
{
	Init();
}
void FxaaController::Init()
{
	this->mFaxxMaterialContext = MaterialContexts::GetMaterialContext(FXAA_CONSOLE_RENDER_SHADER_ID);
	FxaaConstants cb;
	cb.gFxaaConsoleEdgeMainThresholdMin = 0.05;
	cb.gFxaaConsoleEdgeSharpness = 8.0;
	cb.gFxaaConsoleEdgeThreshold = 0.125;
	cb.gFxaaFrameInvSize.x = (1 / (float)mWidth);
	cb.gFxaaFrameInvSize.y = (1 / (float)mHeight);

	this->mFxaaCB.SetData(0, cb);

	mViewPort =
	{
		0,0,
		static_cast<float>(mWidth),
		static_cast<float>(mHeight),
		0.0f,1.0f
	};
	mRect =
	{
		0,0,
		mWidth,mHeight
	};
	this->mRtvDescriptor = mGDevice->RtvDescriptorHeapAllocator->Allocator(1);
	this->mSrvDescritpor = mGDevice->CBV_SRV_UAV_Shader_Visible_DescriptorHeapAllocator->Allocator(1);
	CreateResource();
	BuildDescriptor();
}
void FxaaController::CreateResource()
{
	ResourceDescription desc;
	desc.ClearColor[0] = 0.0f;
	desc.ClearColor[1] = 0.0f;
	desc.ClearColor[2] = 0.0f;
	desc.ClearColor[3] = 0.0f;
	desc.Format = mFxaaFormat;
	desc.Width = this->mWidth ;
	desc.Height = this->mHeight ;
	desc.ResourceState = D3D12_RESOURCE_STATE_GENERIC_READ;
	this->mFxaaTexture= GTexture::CreateRenderTargetTexture(mGDevice, desc);
}

void FxaaController::BuildDescriptor()
{
	this->mRtv = RenderTargetView::CreateRenderTargetView(mGDevice, this->mFxaaTexture.get(),
		mRtvDescriptor, this->mFxaaFormat);
	this->mSrv = ShaderResourceView::CreateShaderView(mGDevice, this->mFxaaTexture.get(), this->mSrvDescritpor, this->mFxaaFormat);
}
void FxaaController::PreRender(ID3D12GraphicsCommandList* mCommandList)
{
	auto before = CD3DX12_RESOURCE_BARRIER::Transition(this->mFxaaTexture->NavigateResource(),
		D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_RENDER_TARGET);
	mCommandList->ResourceBarrier(1, &before);
	float clearValue[4] = { 0.0f,0.0f,0.0f,0.0f };
	auto rtvHandle = this->mRtv->GetCPUView();
	mCommandList->ClearRenderTargetView(rtvHandle, clearValue, 0, nullptr);
	mCommandList->OMSetRenderTargets(1, &rtvHandle, true, nullptr);
	mCommandList->RSSetViewports(1, &this->mViewPort);
	mCommandList->RSSetScissorRects(1, &this->mRect);
}
void FxaaController::EndRender(ID3D12GraphicsCommandList* mCommandList)
{
	auto after = CD3DX12_RESOURCE_BARRIER::Transition(this->mFxaaTexture->NavigateResource(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,D3D12_RESOURCE_STATE_GENERIC_READ);
	mCommandList->ResourceBarrier(1, &after);
}

void FxaaController::OnRender(ID3D12GraphicsCommandList* mCommandList, D3D12_GPU_DESCRIPTOR_HANDLE useSrvMap)
{
	auto rs = this->mFaxxMaterialContext->mRenderShader;
	mCommandList->SetPipelineState(rs->mShaderPasses[0].GetPipline()->mDxPipelineState.Get());
	mCommandList->SetGraphicsRootSignature(rs->mRootSignature->GetRootSig());
	
	//this->mFaxxMaterialContext->SetGraphicsRoot(mCommandList);

	mCommandList->SetGraphicsRootConstantBufferView(0, this->mFxaaCB.NavigateResource()->GetGPUVirtualAddress());
	mCommandList->SetGraphicsRootDescriptorTable(1, useSrvMap);


	mCommandList->IASetVertexBuffers(0, 0, nullptr);
	mCommandList->IASetIndexBuffer(nullptr);
	mCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mCommandList->DrawInstanced(6, 1, 0, 0);

}
void FxaaController::Resize(int width, int height)
{
	if (this->mWidth != width || this->mHeight != height)
	{
		mViewPort =
		{
			0,0,
			static_cast<float>(mWidth),
			static_cast<float>(mHeight),
			0.0f,1.0f
		};
		mRect =
		{
			0,0,
			mWidth,mHeight
		};
		FxaaConstants cb;
		cb.gFxaaConsoleEdgeMainThresholdMin = 0.05;
		cb.gFxaaConsoleEdgeSharpness = 8.0;
		cb.gFxaaConsoleEdgeThreshold = 0.125;
		cb.gFxaaFrameInvSize.x = (1 / (float)mWidth);
		cb.gFxaaFrameInvSize.y = (1 / (float)mHeight);

		this->mFxaaCB.SetData(0, cb);
		CreateResource();
		BuildDescriptor();
	}
}

D3D12_GPU_DESCRIPTOR_HANDLE FxaaController::GetPostProcessedMap()
{
	return mSrv->GetGPUView();
}


