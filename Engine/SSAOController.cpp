#include"SSAOController.h"
#include"ShaderResourceView.h"
#include"GMathHelper.h"
#include"DirectXPackedVector.h"
using namespace DirectX;
SSAOController::SSAOController(
	UINT width, UINT height, 
	GraphicsDevice* device)
	:mGDevice(device),
	mSsaoUploadData(mGDevice,1,true)
{
	mRtvDescriptors[0] = mGDevice->RtvDescriptorHeapAllocator->Allocator(1);
	mRtvDescriptors[1] = mGDevice->RtvDescriptorHeapAllocator->Allocator(1);
	mSrvDescriptors[0] = mGDevice->CBV_SRV_UAV_Shader_Visible_DescriptorHeapAllocator->Allocator(1);
	mSrvDescriptors[1] = mGDevice->CBV_SRV_UAV_Shader_Visible_DescriptorHeapAllocator->Allocator(1);
	mSrvDescriptors[2] = mGDevice->CBV_SRV_UAV_Shader_Visible_DescriptorHeapAllocator->Allocator(1);
	BuildRandomVectorResource();
	BuildRandomVectorDescriptor();
	Resize(width, height);

	mSsaoMaterialContext=MaterialContexts::GetMaterialContext(SSAO_RENDER_SHADER_ID);
	BuildOffsetVectors();
	
}
D3D12_GPU_DESCRIPTOR_HANDLE SSAOController::GetSsaoMap()
{
	return this->mAoSrv1->GetGPUView();
}
void SSAOController::Resize(UINT width, UINT height)
{
	if (this->mWidth!=width||this->mHeight!=height)
	{
		this->mWidth = width;
		this->mHeight = height;
		mViewPort.TopLeftX = 0.0f;
		mViewPort.TopLeftY = 0.0f;
		mViewPort.Width = this->mWidth / 2;
		mViewPort.Height = this->mHeight / 2;
		mViewPort.MinDepth = 0.0f;
		mViewPort.MaxDepth = 1.0f;

		mRect = { 0,0,(int)this->mWidth / 2,(int)this->mHeight / 2 };

		BuildAOResource();
		BuildDescriptor();
	}
}
void SSAOController::BuildAOResource()
{
	ResourceDescription desc;
	desc.ClearColor[0] = 1.0f;
	desc.ClearColor[1] = 1.0f;
	desc.ClearColor[2] = 1.0f;
	desc.ClearColor[3] = 1.0f;
	desc.Format = mAOMapFormat;
	desc.Width = this->mWidth / 2;
	desc.Height = this->mHeight / 2;
	desc.ResourceState = D3D12_RESOURCE_STATE_GENERIC_READ;
	this->mAOMap1= GTexture::CreateRenderTargetTexture(mGDevice, desc);
	this->mAOMap2 = GTexture::CreateRenderTargetTexture(mGDevice, desc);
}
void SSAOController::BuildDescriptor()
{
	mAoSrv1 = ShaderResourceView::CreateShaderView(mGDevice, mAOMap1.get(), mSrvDescriptors[AoMap1_SrvIndex], this->mAOMapFormat);
	mAoSrv2 = ShaderResourceView::CreateShaderView(mGDevice, mAOMap2.get(), mSrvDescriptors[AoMap2_SrvIndex], this->mAOMapFormat);

	mAoRtv1 = RenderTargetView::CreateRenderTargetView(mGDevice, mAOMap1.get(), mRtvDescriptors[AoMap1_RtvIndex], this->mAOMapFormat);
	mAoRtv2 = RenderTargetView::CreateRenderTargetView(mGDevice, mAOMap2.get(), mRtvDescriptors[AoMap2_RtvIndex], this->mAOMapFormat);
}




void SSAOController::BuildRandomVectorDescriptor()
{

	this->mRandomVectorSrv= ShaderResourceView::CreateShaderView(mGDevice, this->mRandomVectorMap.get(), mSrvDescriptors[RandomVector_SrvIndex], this->mRandomVectorMapFormat);
}
void SSAOController::BuildRandomVectorResource()
{
	D3D12_RESOURCE_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(D3D12_RESOURCE_DESC));
	texDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	texDesc.Alignment = 0;
	texDesc.Width = 256;
	texDesc.Height = 256;
	texDesc.DepthOrArraySize = 1;
	texDesc.MipLevels = 1;
	texDesc.Format = this->mRandomVectorMapFormat;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	texDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
	this->mRandomVectorMap = GTexture::CreateTexture(mGDevice, D3D12_RESOURCE_STATE_GENERIC_READ, &texDesc);

	const UINT num2DSubresources = texDesc.DepthOrArraySize * texDesc.MipLevels;
	const UINT64 uploadBufferSize = GetRequiredIntermediateSize(mRandomVectorMap->NavigateResource(), 0, num2DSubresources);
	ComPtr<ID3D12Resource> mRandomVectorMapUploadBuffer;
	auto properties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize);
	ThrowGraphicsfFailed(mGDevice->mD3dDevice->CreateCommittedResource(
		&properties,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(mRandomVectorMapUploadBuffer.GetAddressOf())));


	DirectX::PackedVector::XMCOLOR initData[256 * 256];
	for (int i = 0; i < 256; i++)
	{
		for (int j = 0; j < 256; j++)
		{
			DirectX::XMFLOAT3 v(GMathHelper::RandF(), GMathHelper::RandF(), GMathHelper::RandF());
			initData[i * 256 + j] = DirectX::PackedVector::XMCOLOR(v.x, v.y, v.z,0.0f);
		}
	}
	


	mGDevice->FlushCommandQueue();
	mGDevice->ResetDirectCommandAlloc();
	mGDevice->ResetDirectCommandList();
	auto mCommandList=mGDevice->mDirectCommandList;
	D3D12_SUBRESOURCE_DATA subResourceData = {};
	subResourceData.pData = initData;
	subResourceData.RowPitch = 256 * sizeof(DirectX::PackedVector::XMCOLOR);
	subResourceData.SlicePitch = subResourceData.RowPitch * 256;

	auto before=CD3DX12_RESOURCE_BARRIER::Transition(this->mRandomVectorMap->NavigateResource(),
		D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_COPY_DEST);
	mCommandList->ResourceBarrier(1, &before);
	UpdateSubresources(mCommandList.Get(), mRandomVectorMap->NavigateResource(), mRandomVectorMapUploadBuffer.Get(),
		0, 0, num2DSubresources, &subResourceData);

	auto after=CD3DX12_RESOURCE_BARRIER::Transition(this->mRandomVectorMap->NavigateResource(),
		D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ);
	mCommandList->ResourceBarrier(1, &after);
	mGDevice->ExctureCommands();
	mGDevice->FlushCommandQueue();

}

void SSAOController::BeginSSAORender(ID3D12GraphicsCommandList* mCommandList)
{
	CD3DX12_RESOURCE_BARRIER before = CD3DX12_RESOURCE_BARRIER::Transition(this->mAOMap1->NavigateResource(),
		D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_RENDER_TARGET);
	mCommandList->ResourceBarrier(1, &before);

	float clearValue[] = { 1.0f,1.0f,1.0f,1.0f };
	auto RtvHandle = this->mAoRtv1->GetCPUView();
	mCommandList->ClearRenderTargetView(RtvHandle, clearValue, 0, nullptr);
	mCommandList->OMSetRenderTargets(1, &RtvHandle, true, nullptr);

	mCommandList->RSSetViewports(1, &mViewPort);
	mCommandList->RSSetScissorRects(1, &mRect);
}
void SSAOController::OnRenderSSAO(ID3D12GraphicsCommandList* mCommandList, D3D12_GPU_DESCRIPTOR_HANDLE normalMap,
	D3D12_GPU_DESCRIPTOR_HANDLE depthMap)
{
	auto rs=this->mSsaoMaterialContext->mRenderShader;
	mCommandList->SetPipelineState(rs->mShaderPasses[0].GetPipline()->mDxPipelineState.Get());
	mCommandList->SetGraphicsRootSignature(rs->mRootSignature->GetRootSig());
	mCommandList->SetGraphicsRootConstantBufferView(0, this->mSsaoUploadData.NavigateResource()->GetGPUVirtualAddress());
	mCommandList->SetGraphicsRoot32BitConstant(1, 0, 0);
	mCommandList->SetGraphicsRootDescriptorTable(2, normalMap);
	mCommandList->SetGraphicsRootDescriptorTable(3, depthMap);
	mCommandList->SetGraphicsRootDescriptorTable(4, this->mRandomVectorSrv->GetGPUView());
	
	mCommandList->IASetVertexBuffers(0, 0, nullptr);
	mCommandList->IASetIndexBuffer(nullptr);
	mCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mCommandList->DrawInstanced(6, 1, 0, 0);
}
void SSAOController::EndSSAORender(ID3D12GraphicsCommandList* mCommandList)
{
	CD3DX12_RESOURCE_BARRIER after = CD3DX12_RESOURCE_BARRIER::Transition(this->mAOMap1->NavigateResource(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_GENERIC_READ);
	mCommandList->ResourceBarrier(1, &after);
}
void SSAOController::UpdateConstantBuffer(CommonPassConstants &mainCB)
{
	auto temp = DirectX::XMLoadFloat4x4(&mainCB.Proj);
	XMMATRIX P = DirectX::XMMatrixTranspose(temp);

	DirectX::XMMATRIX T(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f);
	

	this->mSsaoCB.Proj = mainCB.Proj;
	mSsaoCB.InvProj = mainCB.InvProj;
	XMStoreFloat4x4(&mSsaoCB.ProjTex, XMMatrixTranspose(P * T));

	this->GetOffsetVectors(mSsaoCB.OffsetVectors);

	auto blurWeights = CalcGaussWeights(2.5f);
	mSsaoCB.BlurWeights[0] = XMFLOAT4(&blurWeights[0]);
	mSsaoCB.BlurWeights[1] = XMFLOAT4(&blurWeights[4]);
	mSsaoCB.BlurWeights[2] = XMFLOAT4(&blurWeights[8]);

	mSsaoCB.InvRenderTargetSize = XMFLOAT2(1.0f / mWidth, 1.0f / mHeight);

	// Coordinates given in view space.
	mSsaoCB.OcclusionRadius = 0.5f;
	mSsaoCB.OcclusionFadeStart = 0.2f;
	mSsaoCB.OcclusionFadeEnd = 1.0f;
	mSsaoCB.SurfaceEpsilon = 0.05f;

	
	this->mSsaoUploadData.SetData(0, mSsaoCB);
}






void SSAOController::BeginSSAOBlurRender(ID3D12GraphicsCommandList* mCommandList)
{
}
void SSAOController::EndSSAOBlurRender(ID3D12GraphicsCommandList* mCommandList)
{
}


void SSAOController::BuildOffsetVectors()
{
	// 8 cube corners
	mOffsets[0] = DirectX::XMFLOAT4(+1.0f, +1.0f, +1.0f, 0.0f);
	mOffsets[1] = DirectX::XMFLOAT4(-1.0f, -1.0f, -1.0f, 0.0f);

	mOffsets[2] = DirectX::XMFLOAT4(-1.0f, +1.0f, +1.0f, 0.0f);
	mOffsets[3] = DirectX::XMFLOAT4(+1.0f, -1.0f, -1.0f, 0.0f);

	mOffsets[4] = DirectX::XMFLOAT4(+1.0f, +1.0f, -1.0f, 0.0f);
	mOffsets[5] = DirectX::XMFLOAT4(-1.0f, -1.0f, +1.0f, 0.0f);

	mOffsets[6] = DirectX::XMFLOAT4(-1.0f, +1.0f, -1.0f, 0.0f);
	mOffsets[7] = DirectX::XMFLOAT4(+1.0f, -1.0f, +1.0f, 0.0f);

	// 6 centers of cube faces
	mOffsets[8] = DirectX::XMFLOAT4(-1.0f, 0.0f, 0.0f, 0.0f);
	mOffsets[9] = DirectX::XMFLOAT4(+1.0f, 0.0f, 0.0f, 0.0f);

	mOffsets[10] = DirectX::XMFLOAT4(0.0f, -1.0f, 0.0f, 0.0f);
	mOffsets[11] = DirectX::XMFLOAT4(0.0f, +1.0f, 0.0f, 0.0f);

	mOffsets[12] = DirectX::XMFLOAT4(0.0f, 0.0f, -1.0f, 0.0f);
	mOffsets[13] = DirectX::XMFLOAT4(0.0f, 0.0f, +1.0f, 0.0f);

	for (int i = 0; i < 14; ++i)
	{
		// Create random lengths in [0.25, 1.0].
		float s = GMathHelper::RandF(0.25f, 1.0f);
		
		DirectX::XMVECTOR v = s * (DirectX::XMVector4Normalize(XMLoadFloat4(&mOffsets[i])));

		XMStoreFloat4(&mOffsets[i], v);
	}
}
