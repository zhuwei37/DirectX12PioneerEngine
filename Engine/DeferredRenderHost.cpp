#include"DeferredRenderHost.h"
#include"GeometryGenerator.h"
#include"DirectXMath.h"

using namespace DirectX;
DeferredRenderHost::DeferredRenderHost(GraphicsDevice* GDevice, CreateRenderHostParameter* param)
	:RenderHost(GDevice,param),
	mCommonShadowPassConstantsRenderData(mGDevice,1,true)
{
	deferredMaterialContext = MaterialContexts::GetMaterialContext(DEFERRED_RENDER_SHADER_ID);
	screenQuadMaterialContext = MaterialContexts::GetMaterialContext(SCREEN_QUAD_RENDER_SHADER_ID);
	deferredLightMaterialContext = MaterialContexts::GetMaterialContext(DEFERRED_LIGHT_RENDER_SHADER_ID);
	shadowMapMaterialContext = MaterialContexts::GetMaterialContext(SHADOW_MAP_RENDER_SHADER_ID);
	skyBoxMaterialContext = MaterialContexts::GetMaterialContext(SKYBOX_RENDER_SHADER_ID);


	auto id = GUIDHelper::StringToResourceId(DEFAULT_ENVIRONMENT_MAP_TEXTURE_RESOURCEID_STR);
	auto EnvaironmentTex =(TextureResource*) EngineManager::Get()->GetResourceSystem()->GetResourceManager()->GetResource(id);
	skyBoxMaterialContext->SetTexture(ShaderProperty::PropertyToID("EnvironmentMapping"), EnvaironmentTex->GetTexture());

	auto  matid = GUIDHelper::StringToResourceId(SKYBOX_MATERIAL_RESOURCE_STR);
	auto mat = dynamic_cast<MaterialResource*>(EngineManager::Get()->GetResourceSystem()->GetResourceManager()->GetResource(matid));
	this->mSkyBoxMat = mat->GetMaterial();

	mShadowMapController = std::make_shared<ShadowMapController>(GDevice);
	mShadowMap = std::make_shared<ShadowMap>(GDevice->mD3dDevice.Get(), 2048, 2048);
	

	mSkybox = std::make_shared<SkyBox>();
	

	auto dsvHandle = GDevice->DsvDescriptorHeapAllocator->Allocator(1);
	auto srvHandle = GDevice->CBV_SRV_UAV_Shader_Visible_DescriptorHeapAllocator->Allocator(1);

	mShadowMap->BuildDescriptors(CD3DX12_CPU_DESCRIPTOR_HANDLE(srvHandle.CpuHandle),
		CD3DX12_GPU_DESCRIPTOR_HANDLE(srvHandle.GpuHandle),
		CD3DX12_CPU_DESCRIPTOR_HANDLE (dsvHandle.CpuHandle));
	mSceneBounds.Center = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	mSceneBounds.Radius = sqrtf(10.0f * 10.0f + 15.0f * 15.0f);


	CreateMRTResource();
	CreateMRTDescriptor();
	MRTResourceBindDescriptor();
	CreateScreenQuadMesh();


}


void DeferredRenderHost::PreRender(std::map<Layer, RendererList>& renderers, SkyBox* skybox)
{
	RenderHost::PreRender(renderers,skybox);
	this->UpdateShadowTransform();
	this->UpdateCommonShadowPassConstants();
	auto shadowTransform = DirectX::XMLoadFloat4x4(&this->mShadowTransform);
	DirectX::XMStoreFloat4x4(&this->mCommonPass.ShadowTransform, DirectX::XMMatrixTranspose(shadowTransform));
	this->mCommonPassConstantsRenderData.SetData(0, this->mCommonPass);
	

	




}

void DeferredRenderHost::CreateScreenQuadMesh()
{
	GeometryGenerator creator;
	auto worldPosQuad= creator.CreateQuad(0.5f, 0.0f, 0.5f, 1.0f, 0.0f);
	this->woroldPosQuadMesh = std::make_shared<Mesh>(worldPosQuad, mGDevice);

	auto normalRoughness = creator.CreateQuad(0.0, 0.0f, 0.5f, 1.0f, 0.0f);
	this->normalRoughnessQuadMesh = std::make_shared<Mesh>(normalRoughness, mGDevice);

	auto diffuse = creator.CreateQuad(-0.5f, 0.0f, 0.5f, 1.0f, 0.0f);
	this->diffuseQuadMesh = std::make_shared<Mesh>(diffuse, mGDevice);

	auto dpecularAndEmissive = creator.CreateQuad(-1.0f, 0.0f, 0.5f, 1.0f, 0.0f);
	this->dpecularAndEmissiveQuadMesh = std::make_shared<Mesh>(dpecularAndEmissive, mGDevice);

	auto light = creator.CreateQuad(-1.0f, 1.0f, 2.0f, 1.0f, 0.0f);
	this->lightQuadMesh = std::make_shared<Mesh>(light, mGDevice);

	auto screen = creator.CreateQuad(-1.0f, 1.0f, 2.0f, 2.0f, 0.0f);
	this->ScreenQuadMesh = std::make_shared<Mesh>(screen, mGDevice);

}



//渲染
ComPtr<ID3D12GraphicsCommandList> DeferredRenderHost::OnRender()
{
	//return RenderHost::OnRender();


	auto commandList = this->mCommandList;

	//重置命令列表
	this->mCommandAllocator->Reset();
	this->mCommandList->Reset(this->mCommandAllocator.Get(), nullptr);
	ID3D12DescriptorHeap* descriptorHeaps[] = 
	{ mGDevice->CBV_SRV_UAV_Shader_Visible_DescriptorHeapAllocator->GetNavigateDescriptorHeap() 
		
	};
	commandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);
	RenderShadowMap();
	RenderGBuffer();

	RenderLight();
	RenderSkyBox();
	for (int i = GBufferRTVOrder::Light; i < GBufferRTVOrder::Length; i++)
	{
		D3D12_RESOURCE_BARRIER mrtResourceBar = CD3DX12_RESOURCE_BARRIER::Transition(this->MRTResources[i]->NavigateResource(),
			D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_GENERIC_READ);
		this->mCommandList->ResourceBarrier(1, &mrtResourceBar);
	}
	////设置视口
	commandList->RSSetViewports(1, &mViewPort);
	commandList->RSSetScissorRects(1, &mRect);
	//// 设置RTV、DSV
	////设置资源屏障
	D3D12_RESOURCE_BARRIER rtvResourceBar = CD3DX12_RESOURCE_BARRIER::Transition(mPresenter->GetCurrentBuffer(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	commandList->ResourceBarrier(1, &rtvResourceBar);
    float color[4] = { 0.0f,0.0f,0.0f,0.0f };
	commandList->ClearRenderTargetView(mPresenter->GetCurrentBufferView(), DirectX::Colors::LightBlue, 0, nullptr);
	//commandList->ClearDepthStencilView(mDsv->GetCPUView(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);
	auto curBackBufferAddr = mPresenter->GetCurrentBufferView();
	auto curDsvAddr = mDsv->GetCPUView();
	commandList->OMSetRenderTargets(1, &curBackBufferAddr, false,nullptr);
	
	

    RenderWorldPosQuad();
	//设置资源屏障
	D3D12_RESOURCE_BARRIER rtvResourceBar2 = CD3DX12_RESOURCE_BARRIER::Transition(mPresenter->GetCurrentBuffer(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	commandList->ResourceBarrier(1, &rtvResourceBar2);

	ThrowGraphicsfFailed(commandList->Close());
	//结束
	return commandList;
	
}



/// <summary>
/// 天空盒
/// </summary>
void DeferredRenderHost::RenderSkyBox()
{
	//auto handle2 = CD3DX12_CPU_DESCRIPTOR_HANDLE(MRT_RtvDescriptorHandle.CpuHandle);
	//handle2.Offset(GBufferRTVOrder::Light * mGDevice->RTV_DescriptorSize);
	//auto dsv = this->mDsv->GetCPUView();
	//this->mCommandList->OMSetRenderTargets(1, &handle2, true, &dsv);



	//auto mesh = this->mSkybox->GetMesh();
	//if (mesh == nullptr || mesh->SubMeshes.size() < 0)
	//{
	//	return;
	//}
	//
	////设置顶点 顶点索引
	//auto vbv = mesh->VertexBufferView();
	//this->mCommandList->IASetVertexBuffers(0, 1, &vbv);
	//auto ibv = mesh->IndexBufferView();
	//this->mCommandList->IASetIndexBuffer(&ibv);
	//this->mCommandList->IASetPrimitiveTopology(mesh->PrimitiveTopology);
	//for (int i = 0; i < mesh->SubMeshes.size(); i++)
	//{
	//	auto subMesh = mesh->SubMeshes[i];
	//	//启用默认材质
	//	
	//	
	//	{
	//		auto mat = this->mSkyBoxMat;
	//		if (mat == nullptr)
	//		{
	//			continue;
	//		}
	//		auto gMat = mat->GetGMat();
	//		if (gMat != nullptr)
	//		{

	//			auto rootSignature = gMat->mRenderShader->mRootSignature;
	//			if (rootSignature == nullptr)
	//			{
	//				continue;
	//			}
	//			this->mCommandList->SetGraphicsRootSignature(rootSignature->GetRootSig());
	//			//固定参数
	//			//裁剪矩阵、环境光
	//			this->mCommandList->SetGraphicsRootConstantBufferView(0, this->mCommonPassConstantsRenderData.NavigateResource()->GetGPUVirtualAddress());
	//			//光源
	//			this->mCommandList->SetGraphicsRootConstantBufferView(1, this->mCommonLightRenderData.NavigateResource()->GetGPUVirtualAddress());


	//			auto size = (sizeof(ObjectPassConstants) + 255) & ~255;
	//			this->mCommandList->SetGraphicsRootConstantBufferView(2
	//				, this->mCommonObjectPassContantsRenderData.NavigateResource()
	//				->GetGPUVirtualAddress() + this->mSkyObjIndex * size);
	//			//自定义着色器参数
	//			gMat->SetGraphicsRoot(this->mCommandList.Get());

	//			for (auto& pass : gMat->mRenderShader->mShaderPasses)
	//			{
	//				this->mCommandList->SetPipelineState(pass.GetPipline()->mDxPipelineState.Get());
	//				{
	//					//触发DrawCall命令
	//					this->mCommandList->DrawIndexedInstanced(subMesh.IndexCount, 1, subMesh.IndexBaseStart, subMesh.VertexBaseStart, 0);
	//				}
	//			}
	//		}
	//	}
	//}
	auto handle2 = CD3DX12_CPU_DESCRIPTOR_HANDLE(MRT_RtvDescriptorHandle.CpuHandle);
	handle2.Offset(GBufferRTVOrder::Light * mGDevice->RTV_DescriptorSize);
	auto dsv = this->mDsv->GetCPUView();
	this->mCommandList->OMSetRenderTargets(1, &handle2, true, &dsv);






	Mesh* mesh = this->mSkybox->GetMesh();
	auto vbv = mesh->VertexBufferView();
	this->mCommandList->IASetVertexBuffers(0, 1, &vbv);
	auto ibv = mesh->IndexBufferView();
	this->mCommandList->IASetIndexBuffer(&ibv);
	this->mCommandList->IASetPrimitiveTopology(mesh->PrimitiveTopology);

	for (int i = 0; i < mesh->SubMeshes.size(); i++)
	{
		auto skyBoxMaterialContext = this->mSkyBoxMat->GetGMat();

		 auto rootSignature = skyBoxMaterialContext->mRenderShader->mRootSignature;

		this->mCommandList->SetGraphicsRootSignature(rootSignature->GetRootSig());



		this->mCommandList->SetGraphicsRootConstantBufferView(0,
			this->mCommonPassConstantsRenderData.NavigateResource()->GetGPUVirtualAddress());

		this->mCommandList->SetGraphicsRootConstantBufferView(1, this->mCommonLightRenderData.NavigateResource()->GetGPUVirtualAddress());


		auto size = (sizeof(ObjectPassConstants) + 255) & ~255;
		this->mCommandList->SetGraphicsRootConstantBufferView(2
			, this->mCommonObjectPassContantsRenderData.NavigateResource()
			->GetGPUVirtualAddress() + this->mSkyObjIndex * size);

		this->skyBoxMaterialContext->SetGraphicsRoot(this->mCommandList.Get());


		auto subMesh = mesh->SubMeshes[i];
		for (auto& pass : skyBoxMaterialContext->mRenderShader->mShaderPasses)
		{
			this->mCommandList->SetPipelineState(pass.GetPipline()->mDxPipelineState.Get());
			{

				this->mCommandList->DrawIndexedInstanced(subMesh.IndexCount, 1, subMesh.IndexBaseStart, subMesh.VertexBaseStart, 0);
			}
		}
	}
	//for (int i = 0; i < mesh->SubMeshes.size(); i++)
	//{
	//	auto subMesh = mesh->SubMeshes[i];
	//	//启用默认材质



	//	auto mat = this->mSkyBoxMat;

	//	auto gMat = mat->GetGMat();


	//	auto rootSignature = gMat->mRenderShader->mRootSignature;

	//	this->mCommandList->SetGraphicsRootSignature(rootSignature->GetRootSig());
	//	//固定参数
	//	//裁剪矩阵、环境光
	//	this->mCommandList->SetGraphicsRootConstantBufferView(0, this->mCommonPassConstantsRenderData.NavigateResource()->GetGPUVirtualAddress());
	//	//光源
	//	this->mCommandList->SetGraphicsRootConstantBufferView(1, this->mCommonLightRenderData.NavigateResource()->GetGPUVirtualAddress());


	//	auto size = (sizeof(ObjectPassConstants) + 255) & ~255;
	//	this->mCommandList->SetGraphicsRootConstantBufferView(2
	//		, this->mCommonObjectPassContantsRenderData.NavigateResource()
	//		->GetGPUVirtualAddress() + this->mSkyObjIndex * size);
	//	//自定义着色器参数
	//	gMat->SetGraphicsRoot(this->mCommandList.Get());

	//	for (auto& pass : gMat->mRenderShader->mShaderPasses)
	//	{
	//		this->mCommandList->SetPipelineState(pass.GetPipline()->mDxPipelineState.Get());
	//		{
	//			//触发DrawCall命令
	//			this->mCommandList->DrawIndexedInstanced(subMesh.IndexCount, 1, subMesh.IndexBaseStart, subMesh.VertexBaseStart, 0);
	//		}
	//	}
	//}



}
void DeferredRenderHost::Resize(int width, int height)
{
	RenderHost::Resize(width, height);
	this->CreateMRTResource();
	this->MRTResourceBindDescriptor();
}

void DeferredRenderHost::CreateMRTResource()
{
	ResourceDescription resourceDescription;
	resourceDescription.Height = mHeight;
	resourceDescription.Width = mWidth;
	resourceDescription.ClearColor[0] = 0.0f;
	resourceDescription.ClearColor[1] = 0.0f;
	resourceDescription.ClearColor[2] = 0.0f;
	resourceDescription.ClearColor[3] = 0.0f;
	resourceDescription.ResourceState = D3D12_RESOURCE_STATE_GENERIC_READ;
	for (int i = 0; i < GBufferRTVOrder::Length; i++)
	{
		resourceDescription.Format = MRT_Formats[i]; 
		MRTResources[i]=GTexture::CreateRenderTargetTexture(mGDevice, resourceDescription);
	}
}
void DeferredRenderHost::CreateMRTDescriptor()
{
	this->MRT_SrvDescriptorHandle = mGDevice->CBV_SRV_UAV_Shader_Visible_DescriptorHeapAllocator->Allocator(GBufferRTVOrder::Length);
	this->MRT_RtvDescriptorHandle = mGDevice->RtvDescriptorHeapAllocator->Allocator(GBufferRTVOrder::Length);

}
void DeferredRenderHost::RenderWorldPosQuad()
{
	
	this->mCommandList->SetPipelineState(this->screenQuadMaterialContext->mRenderShader->mShaderPasses[0].GetPipline()->mDxPipelineState.Get());
	this->mCommandList->SetGraphicsRootSignature(this->screenQuadMaterialContext->mRenderShader->mRootSignature->GetRootSig());

	//debug显示g-buffer数据
	CD3DX12_GPU_DESCRIPTOR_HANDLE MRTHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(MRT_SrvDescriptorHandle.GpuHandle);
	auto MRTHandle2 = MRTHandle;
	//MRTHandle.Offset(1*mGDevice->CBV_SRV_UAV_DescriptorSize);
	this->mCommandList->SetGraphicsRootDescriptorTable(0, MRTHandle);
	//DrawMesh(this->woroldPosQuadMesh.get());

	MRTHandle.Offset(1 * mGDevice->CBV_SRV_UAV_DescriptorSize);
	this->mCommandList->SetGraphicsRootDescriptorTable(0, MRTHandle);
	//DrawMesh(this->normalRoughnessQuadMesh.get());
	
	MRTHandle.Offset(1 * mGDevice->CBV_SRV_UAV_DescriptorSize);
	this->mCommandList->SetGraphicsRootDescriptorTable(0, MRTHandle);
	//DrawMesh(this->diffuseQuadMesh.get());

	

	MRTHandle.Offset(1 * mGDevice->CBV_SRV_UAV_DescriptorSize);
	/*this->mCommandList->SetGraphicsRootDescriptorTable(0, MRTHandle);
	DrawMesh(this->dpecularAndEmissiveQuadMesh.get());*/

	auto shadowMapHandle = this->mShadowMapController->GetShadowMapSrvGPUHandle();
	this->mCommandList->SetGraphicsRootDescriptorTable(0, shadowMapHandle);
	//DrawMesh(this->dpecularAndEmissiveQuadMesh.get());

	MRTHandle.Offset(1 * mGDevice->CBV_SRV_UAV_DescriptorSize);
	
	this->mCommandList->SetGraphicsRootDescriptorTable(0, MRTHandle);
	DrawMesh(ScreenQuadMesh.get());
	

}

void DeferredRenderHost::DrawMesh(Mesh* mesh)
{
	auto vbv = mesh->VertexBufferView();
	this->mCommandList->IASetVertexBuffers(0, 1, &vbv);
	auto ibv = mesh->IndexBufferView();
	this->mCommandList->IASetIndexBuffer(&ibv);
	this->mCommandList->IASetPrimitiveTopology(mesh->PrimitiveTopology);


	for (auto& subMesh : mesh->SubMeshes)
	{
		this->mCommandList->DrawIndexedInstanced(subMesh.IndexCount, 1, subMesh.IndexBaseStart, subMesh.VertexBaseStart, 0);
	}
}




void DeferredRenderHost::MRTResourceBindDescriptor()
{
	CD3DX12_CPU_DESCRIPTOR_HANDLE handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(MRT_SrvDescriptorHandle.CpuHandle);
	D3D12_SHADER_RESOURCE_VIEW_DESC desc = {};
	desc.Texture2D.MipLevels = 1;
	desc.Texture2D.MostDetailedMip = 0;
	desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	for (int i = 0; i < GBufferRTVOrder::Length; i++)
	{
		desc.Format = MRT_Formats[i];
		mGDevice->mD3dDevice->CreateShaderResourceView(MRTResources[i]->NavigateResource(), &desc, handle);
		handle.Offset(mGDevice->CBV_SRV_UAV_DescriptorSize);
	}

	D3D12_RENDER_TARGET_VIEW_DESC desc2;
	ZeroMemory(&desc, sizeof(desc));
	desc2.Texture2D.PlaneSlice = 0;
	desc2.Texture2D.MipSlice = 0;
	desc2.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	CD3DX12_CPU_DESCRIPTOR_HANDLE handle2 = CD3DX12_CPU_DESCRIPTOR_HANDLE(MRT_RtvDescriptorHandle.CpuHandle);

	for (int i = 0; i < GBufferRTVOrder::Length; i++)
	{
		desc2.Format = MRT_Formats[i];
		mGDevice->mD3dDevice->CreateRenderTargetView(MRTResources[i]->NavigateResource(), &desc2, handle2);
		handle2.Offset(mGDevice->RTV_DescriptorSize);
	}
}
void DeferredRenderHost::RenderShadowMap()
{
	
	this->mShadowMapController->BeginRenderShadowMap(this->mCommandList.Get());
	
	this->mCommandList->SetPipelineState(shadowMapMaterialContext->mRenderShader->mShaderPasses[0].GetPipline()->mDxPipelineState.Get());
	this->mCommandList->SetGraphicsRootSignature(shadowMapMaterialContext->mRenderShader->mRootSignature->GetRootSig());
	this->mCommandList->SetGraphicsRootConstantBufferView(0, mCommonShadowPassConstantsRenderData.Resource->GetGPUVirtualAddress());

	for (auto renderer : mActivityRenderers)
	{
		int objectConstantIndex = renderer->GetObjectPassContantIndex();;
		auto size = (sizeof(ObjectPassConstants) + 255) & ~255;
		this->mCommandList->SetGraphicsRootConstantBufferView(2
			, this->mCommonObjectPassContantsRenderData.NavigateResource()
			->GetGPUVirtualAddress() + objectConstantIndex * size);

		auto meshRenderer = dynamic_cast<MeshRenderer*>(renderer);
		auto mesh = meshRenderer->GetMesh();
		if (mesh != nullptr)
		{
			DrawMesh(mesh);
		}
		
	}
	

	this->mShadowMapController->EndRenderShadowMap(this->mCommandList.Get());
}


/// <summary>
/// GBuffer
/// </summary>
void DeferredRenderHost::RenderGBuffer()
{
	//pre render
	float color[4] = { 0.0f,0.0f,0.0f,0.0f };
	CD3DX12_CPU_DESCRIPTOR_HANDLE handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(MRT_RtvDescriptorHandle.CpuHandle);
	for (int i = 0; i < GBufferRTVOrder::GBufferLength; i++)
	{
		D3D12_RESOURCE_BARRIER mtvResourceBar = CD3DX12_RESOURCE_BARRIER::Transition(this->MRTResources[i]->NavigateResource(),
			D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_RENDER_TARGET);
		this->mCommandList->ResourceBarrier(1, &mtvResourceBar);

		this->mCommandList->ClearRenderTargetView(handle, color, 0, nullptr);
		
		
		handle.Offset(mGDevice->RTV_DescriptorSize);
	}

	this->mCommandList->RSSetViewports(1, &mViewPort);
	this->mCommandList->RSSetScissorRects(1, &mRect);

	//render
	auto handle2 = CD3DX12_CPU_DESCRIPTOR_HANDLE(MRT_RtvDescriptorHandle.CpuHandle);
	this->mCommandList->ClearDepthStencilView(this->mDsv->GetCPUView(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);
	auto handle3 = this->mDsv->GetCPUView();
	this->mCommandList->OMSetRenderTargets(GBufferRTVOrder::GBufferLength, &handle2, true, &handle3);



	for (auto renderer : mActivityRenderers)
	{
		auto meshRenderer = dynamic_cast<MeshRenderer*>(renderer);
		auto mesh = meshRenderer->GetMesh();
		//auto mesh = mMeshFilter.lock()->GetMesh();
		if (mesh == nullptr || mesh->SubMeshes.size() < 0)
		{
			continue;
		}
		auto mat = meshRenderer->GetMaterial(0);
		if (mat == nullptr)
		{
			continue;
		}
		auto gMat = mat->GetMaterial()->GetGMat();
		if (gMat != nullptr)
		{
			
			this->mCommandList->SetGraphicsRootSignature(gMat->mRenderShader->mRootSignature->GetRootSig());

			this->mCommandList->SetPipelineState(gMat->mRenderShader->mShaderPasses[0].GetPipline()->mDxPipelineState.Get());


			this->mCommandList->SetGraphicsRootConstantBufferView(0, this->mCommonPassConstantsRenderData.NavigateResource()->GetGPUVirtualAddress());
			//光源
			this->mCommandList->SetGraphicsRootConstantBufferView(1, this->mCommonLightRenderData.NavigateResource()->GetGPUVirtualAddress());
			int objectConstantIndex = renderer->GetObjectPassContantIndex();;
			auto size = (sizeof(ObjectPassConstants) + 255) & ~255;
			this->mCommandList->SetGraphicsRootConstantBufferView(2
				, this->mCommonObjectPassContantsRenderData.NavigateResource()
				->GetGPUVirtualAddress() + objectConstantIndex * size);

			gMat->SetGraphicsRoot(this->mCommandList.Get());

			//设置顶点 顶点索引
			auto vbv = mesh->VertexBufferView();
			this->mCommandList->IASetVertexBuffers(0, 1, &vbv);
			auto ibv = mesh->IndexBufferView();
			this->mCommandList->IASetIndexBuffer(&ibv);
			this->mCommandList->IASetPrimitiveTopology(mesh->PrimitiveTopology);


			for (auto& subMesh : mesh->SubMeshes)
			{
				this->mCommandList->DrawIndexedInstanced(subMesh.IndexCount, 1, subMesh.IndexBaseStart, subMesh.VertexBaseStart, 0);
			}
		}
	}
	//end read
	for (int i = 0; i < GBufferRTVOrder::GBufferLength; i++)
	{
		D3D12_RESOURCE_BARRIER mrtResourceBar = CD3DX12_RESOURCE_BARRIER::Transition(this->MRTResources[i]->NavigateResource(),
			D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_GENERIC_READ);
		this->mCommandList->ResourceBarrier(1, &mrtResourceBar);
	}

}
void DeferredRenderHost::UpdateCommonShadowPassConstants()
{


	CommonPassConstants shadowPass;
	DirectX::XMMATRIX view = DirectX::XMLoadFloat4x4(&mLightView);
	DirectX::XMMATRIX proj = DirectX::XMLoadFloat4x4(&mLightProj);

	DirectX::XMMATRIX viewProj = DirectX::XMMatrixMultiply(view, proj);

	auto matrix = DirectX::XMMatrixDeterminant(view);
	DirectX::XMMATRIX invView = DirectX::XMMatrixInverse(&matrix, view);

	matrix = DirectX::XMMatrixDeterminant(proj);
	DirectX::XMMATRIX invProj = DirectX::XMMatrixInverse(&matrix, proj);

	matrix = DirectX::XMMatrixDeterminant(viewProj);
	DirectX::XMMATRIX invViewProj = DirectX::XMMatrixInverse(&matrix, viewProj);

	DirectX::XMStoreFloat4x4(&shadowPass.View, DirectX::XMMatrixTranspose(view));
	DirectX::XMStoreFloat4x4(&shadowPass.InvView, DirectX::XMMatrixTranspose(invView));
	DirectX::XMStoreFloat4x4(&shadowPass.Proj, DirectX::XMMatrixTranspose(proj));
	DirectX::XMStoreFloat4x4(&shadowPass.InvProj, DirectX::XMMatrixTranspose(invProj));
	DirectX::XMStoreFloat4x4(&shadowPass.ViewProj, DirectX::XMMatrixTranspose(viewProj));
	DirectX::XMStoreFloat4x4(&shadowPass.InvViewProj, DirectX::XMMatrixTranspose(invViewProj));
	shadowPass.EyePosW = mLightPosW;


	this->mCommonShadowPassConstantsRenderData.SetData(0, shadowPass);


}
void DeferredRenderHost::UpdateShadowTransform()
{
	auto dir = DirectX::XMFLOAT3{ 0.57735f, -0.57735f, 0.57735f };
	DirectX::XMVECTOR lightDir = DirectX::XMLoadFloat3(&dir);
	DirectX::XMVECTOR lightPos = DirectX::XMVectorScale(lightDir, - 2.0f * mSceneBounds.Radius) ;
	DirectX::XMVECTOR targetPos = DirectX::XMLoadFloat3(&mSceneBounds.Center);
	DirectX::XMVECTOR lightUp = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	DirectX::XMMATRIX lightView = DirectX::XMMatrixLookAtLH(lightPos, targetPos, lightUp);


	DirectX::XMStoreFloat3(&mLightPosW, lightPos);

	DirectX::XMFLOAT3 sphereCenterLS;
	DirectX::XMStoreFloat3(&sphereCenterLS, DirectX::XMVector3TransformCoord(targetPos, lightView));


	float l = sphereCenterLS.x - mSceneBounds.Radius;
	float b = sphereCenterLS.y - mSceneBounds.Radius;
	float n = sphereCenterLS.z - mSceneBounds.Radius;
	float r = sphereCenterLS.x + mSceneBounds.Radius;
	float t = sphereCenterLS.y + mSceneBounds.Radius;
	float f = sphereCenterLS.z + mSceneBounds.Radius;


	DirectX::XMMATRIX lightProj = DirectX::XMMatrixOrthographicOffCenterLH(l, r, b, t, n, f);
	DirectX::XMMATRIX T(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f);
		
	DirectX::XMMATRIX S = lightView * lightProj * T;
	DirectX::XMStoreFloat4x4(&mLightView, lightView);
	DirectX::XMStoreFloat4x4(&mLightProj, lightProj);
	DirectX::XMStoreFloat4x4(&mShadowTransform, S);
	

}
void DeferredRenderHost::RenderLight()
{
	
	float color[4] = { 0.0f,0.0f,0.0f,0.0f };
	CD3DX12_CPU_DESCRIPTOR_HANDLE handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(MRT_RtvDescriptorHandle.CpuHandle);
	handle.Offset(GBufferRTVOrder::Light * mGDevice->RTV_DescriptorSize);
	for (int i = GBufferRTVOrder::Light; i < GBufferRTVOrder::Length; i++)
	{
		D3D12_RESOURCE_BARRIER mtvResourceBar = CD3DX12_RESOURCE_BARRIER::Transition(this->MRTResources[i]->NavigateResource(),
			D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_RENDER_TARGET);
		this->mCommandList->ResourceBarrier(1, &mtvResourceBar);

		this->mCommandList->ClearRenderTargetView(handle, color, 0, nullptr);


		handle.Offset(mGDevice->RTV_DescriptorSize);
	}
	this->mCommandList->RSSetViewports(1, &mViewPort);
	this->mCommandList->RSSetScissorRects(1, &mRect);

	auto handle2 = CD3DX12_CPU_DESCRIPTOR_HANDLE(MRT_RtvDescriptorHandle.CpuHandle);
	handle2.Offset(GBufferRTVOrder::Light * mGDevice->RTV_DescriptorSize);
	
	this->mCommandList->OMSetRenderTargets(1, &handle2, true, nullptr);

	
	CD3DX12_GPU_DESCRIPTOR_HANDLE MRTHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(MRT_SrvDescriptorHandle.GpuHandle);
	
	auto shadowMapHandle = this->mShadowMapController->GetShadowMapSrvGPUHandle();

	this->mCommandList->SetPipelineState(this->deferredLightMaterialContext->mRenderShader->mShaderPasses[0].GetPipline()->mDxPipelineState.Get());
	this->mCommandList->SetGraphicsRootSignature(this->deferredLightMaterialContext->mRenderShader->mRootSignature->GetRootSig());
	this->mCommandList->SetGraphicsRootConstantBufferView(0, this->mCommonPassConstantsRenderData.NavigateResource()->GetGPUVirtualAddress());
	this->mCommandList->SetGraphicsRootConstantBufferView(1, this->mCommonLightRenderData.NavigateResource()->GetGPUVirtualAddress());
	this->mCommandList->SetGraphicsRootDescriptorTable(3, MRTHandle);
	this->mCommandList->SetGraphicsRootDescriptorTable(4, shadowMapHandle);
	DrawMesh(this->ScreenQuadMesh.get());




}
DeferredRenderHost::~DeferredRenderHost()
{
}