////***************************************************************************************
//// CrateApp.cpp by Frank Luna (C) 2015 All Rights Reserved.
////***************************************************************************************
//
//
//
//
//#include"CrateApp.h"
//#pragma comment(lib, "d3dcompiler.lib")
//#pragma comment(lib, "D3D12.lib")
//
//const int gNumFrameResources = 1;
//
//
//
//CrateApp::CrateApp()
//{
//}
//
//CrateApp::~CrateApp()
//{
//    if(md3dDevice != nullptr)
//        FlushCommandQueue();
//}
//
//bool CrateApp::Initialize()
//{
//	
//    if(!D3DApp::Initialize())
//        return false;
//	
//    // Reset the command list to prep for initialization commands.
//  //  ThrowGraphicsfFailed(mCommandList->Reset(mDirectCmdListAlloc.Get(), nullptr));
//	mDevice->ResetDirectCommandList();
//
//
//	LoadTextures();
//   
//    BuildShapeGeometry();
//    BuildRenderItems();
//    BuildFrameResources();
//    BuildPSOs();
//
//    // Execute the initialization commands.
//   /* ThrowGraphicsfFailed(mCommandList->Close());
//    ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
// 
//	mDevice->DirectCommandQueue->Execute(cmdsLists);*/
//	mDevice->ExctureCommands();
//    // Wait until initialization is complete.
//    FlushCommandQueue();
//
//	
//
//    return true;
//}
// 
//void CrateApp::OnResize(int width, int height)
//{
//	mClientWidth = width;
//	mClientHeight = height;
//    D3DApp::OnResize(width,height);
//}
//
//void CrateApp::Update(XMMATRIX view,XMMATRIX proj, XMFLOAT3 pos)
//{
//;
//	
//
//    // Cycle through the circular frame resource array.
// 
//    mCurrFrameResource = mFrameResources[0].get();
//
//    // Has the GPU finished processing the commands of the current frame resource?
//    // If not, wait until the GPU has completed commands up to this fence point.
//   
//	UpdateObjectCBs();
//	UpdateMainPassCB(view,proj,pos);
//}
//
//void CrateApp::Draw()
//{
//	auto cmdListAlloc = mDevice->mDirectCmdListAlloc;
//	auto mCommandList = mDevice->mDirectCommandList;
//    // Reuse the memory associated with command recording.
//    // We can only reset when the associated command lists have finished execution on the GPU.
//    ThrowGraphicsfFailed(cmdListAlloc->Reset());
//
//    // A command list can be reset after it has been added to the command queue via ExecuteCommandList.
//    // Reusing the command list reuses memory.
//   // ThrowGraphicsfFailed(mCommandList->Reset(cmdListAlloc.Get(), mRenderContext->Passes[0].GetPipline()->mDxPipelineState.Get()));
//
//    mCommandList->RSSetViewports(1, &mScreenViewport);
//    mCommandList->RSSetScissorRects(1, &mScissorRect);
//
//    // Indicate a state transition on the resource usage.
//	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(Presenter->GetCurrentBuffer() ,
//		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));
//
//    // Clear the back buffer and depth buffer.
//   mCommandList->ClearRenderTargetView(Presenter->GetCurrentBufferView(), Colors::LightSteelBlue, 0, nullptr);
//    mCommandList->ClearDepthStencilView(mDsv->GetCPUView(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);
//
//    // Specify the buffers we are going to render to.
//    mCommandList->OMSetRenderTargets(1, &Presenter->GetCurrentBufferView(), true, &mDsv->GetCPUView());
//
//	ID3D12DescriptorHeap* descriptorHeaps[] =  { mDevice->CBV_SRV_UAV_No_Shader_Visible_DescriptorHeapAllocator->GetNavigateDescriptorHeap() };
//	mCommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);
//	
////	mCommandList->SetGraphicsRootSignature(mRenderContext->RootSig->GetRootSig());
//
//	auto passCB = mCurrFrameResource->PassCB->Resource();
//	mCommandList->SetGraphicsRootConstantBufferView(2, passCB->GetGPUVirtualAddress());
//
//    DrawRenderItems(mCommandList.Get(), mOpaqueRitems);
//
//#pragma region Copy
//
//
//
//	/*mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
//		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_COPY_SOURCE));
//	sharedTex->CopyTexture(CurrentBackBuffer(), mCommandList.Get());*/
//
//	//mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(sharedTex->GetResource(),
//	//	D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_SOURCE));
//	//mFilter->Execute(mCommandList.Get(), sharedTex->GetResource());
//	//mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(sharedTex->GetResource(),
//	//	D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_COMMON));
//
//
//	/*mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
//		D3D12_RESOURCE_STATE_COPY_SOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET));*/
//
//#pragma endregion
//    // Indicate a state transition on the resource usage.
//	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(Presenter->GetCurrentBuffer(),
//		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));
//
//    // Done recording commands.
//    ThrowGraphicsfFailed(mCommandList->Close());
//
//    // Add the command list to the queue for execution.
//    ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
//   // mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);
//	mDevice->DirectCommandQueue->Execute(cmdsLists);
//    // Swap the back and front buffers
//    //ThrowGraphicsfFailed(mSwapChain->Present(0, 0));
//	Presenter->Present();
//
//    // Add an instruction to the command queue to set a new fence point. 
//    // Because we are on the GPU timeline, the new fence point won't be 
//    // set until the GPU finishes processing all the commands prior to this Signal().
//  //  mCommandQueue->Signal(mFence.Get(), mCurrentFence);
//	//mDevice->DirectCommandQueue->Signal();
//
//	mDevice->DirectCommandQueue->FlushCommandQueue();
//
//
//}
//
//
//
//
//
//
//
//void CrateApp::UpdateObjectCBs()
//{
//	auto currObjectCB = mCurrFrameResource->ObjectCB.get();
//	for(auto& e : mAllRitems)
//	{
//		// Only update the cbuffer data if the constants have changed.  
//		// This needs to be tracked per frame resource.
//		if(e->NumFramesDirty > 0)
//		{
//			XMMATRIX world = XMLoadFloat4x4(&e->World);
//			XMMATRIX texTransform = XMLoadFloat4x4(&e->TexTransform);
//
//			ObjectConstants objConstants;
//			XMStoreFloat4x4(&objConstants.World, XMMatrixTranspose(world));
//			XMStoreFloat4x4(&objConstants.TexTransform, XMMatrixTranspose(texTransform));
//
//			currObjectCB->CopyData(e->ObjCBIndex, objConstants);
//
//			// Next FrameResource need to be updated too.
//			e->NumFramesDirty--;
//		}
//	}
//}
//
//
//void CrateApp::UpdateMainPassCB( XMMATRIX mview, XMMATRIX mproj,XMFLOAT3 pos)
//{
//	XMMATRIX view = mview;
//	XMMATRIX proj = mproj;
//	
//	XMMATRIX viewProj = XMMatrixMultiply(view, proj);
//	XMMATRIX invView = XMMatrixInverse(&XMMatrixDeterminant(view), view);
//	XMMATRIX invProj = XMMatrixInverse(&XMMatrixDeterminant(proj), proj);
//	XMMATRIX invViewProj = XMMatrixInverse(&XMMatrixDeterminant(viewProj), viewProj);
//	/*XMStoreFloat3x3*/
//
//	XMStoreFloat4x4(&mMainPassCB.View, XMMatrixTranspose(view));
//	XMStoreFloat4x4(&mMainPassCB.InvView, XMMatrixTranspose(invView));
//	XMStoreFloat4x4(&mMainPassCB.Proj, XMMatrixTranspose(proj));
//	XMStoreFloat4x4(&mMainPassCB.InvProj, XMMatrixTranspose(invProj));
//	XMStoreFloat4x4(&mMainPassCB.ViewProj, XMMatrixTranspose(viewProj));
//	XMStoreFloat4x4(&mMainPassCB.InvViewProj, XMMatrixTranspose(invViewProj));
//	mMainPassCB.EyePosW = pos;
//	mMainPassCB.RenderTargetSize = XMFLOAT2((float)mClientWidth, (float)mClientHeight);
//	mMainPassCB.InvRenderTargetSize = XMFLOAT2(1.0f / mClientWidth, 1.0f / mClientHeight);
//	mMainPassCB.NearZ = 1.0f;
//	mMainPassCB.FarZ = 1000.0f;
//	mMainPassCB.AmbientLight = { 0.25f, 0.25f, 0.35f, 1.0f };
//	mMainPassCB.Lights[0].Direction = { 0.57735f, -0.57735f, 0.57735f };
//	mMainPassCB.Lights[0].Strength = { 0.6f, 0.6f, 0.6f };
//	mMainPassCB.Lights[1].Direction = { -0.57735f, -0.57735f, 0.57735f };
//	mMainPassCB.Lights[1].Strength = { 0.3f, 0.3f, 0.3f };
//	mMainPassCB.Lights[2].Direction = { 0.0f, -0.707f, -0.707f };
//	mMainPassCB.Lights[2].Strength = { 0.15f, 0.15f, 0.15f };
//
//	auto currPassCB = mCurrFrameResource->PassCB.get();
//	currPassCB->CopyData(0, mMainPassCB);
//}
//
//void CrateApp::LoadTextures()
//{
//
//	ComPtr<ID3D12Resource> resource;
//	ComPtr<ID3D12Resource> uploadHeap;
//
//
//	ThrowGraphicsfFailed(DirectX::CreateDDSTextureFromFile12(md3dDevice.Get(),
//		mDevice->mDirectCommandList.Get(), L"../../Textures/checkboard.dds",
//		resource, uploadHeap));
//	/*mSrv = ShaderResourceView::CreateShaderView(mDevice.get(),
//		resource, 
//		uploadHeap);*/
//}
//
//
//void CrateApp::BuildShapeGeometry()
//{
//   /* GeometryGenerator geoGen;
//	GeometryGenerator::MeshData box = geoGen.CreateBox(1.0f, 1.0f, 1.0f, 3);
// 
//	SubmeshGeometry boxSubmesh;
//	boxSubmesh.IndexCount = (UINT)box.Indices32.size();
//	boxSubmesh.StartIndexLocation = 0;
//	boxSubmesh.BaseVertexLocation = 0;
//
// 
//	std::vector<Vertex> vertices(box.Vertices.size());
//
//	for(size_t i = 0; i < box.Vertices.size(); ++i)
//	{
//		vertices[i].Pos = box.Vertices[i].Position;
//		vertices[i].Normal = box.Vertices[i].Normal;
//		vertices[i].TexC = box.Vertices[i].TexC;
//	}
//
//	std::vector<std::uint16_t> indices = box.GetIndices16();
//
//    const UINT vbByteSize = (UINT)vertices.size() * sizeof(Vertex);
//    const UINT ibByteSize = (UINT)indices.size()  * sizeof(std::uint16_t);
//
//	auto geo = std::make_unique<MeshGeometry>();
//	geo->Name = "boxGeo";
//
//
//	geo->VertexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice.Get(),
//		mDevice->mDirectCommandList.Get(), vertices.data(), vbByteSize, geo->VertexBufferUploader);
//
//	geo->IndexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice.Get(),
//		mDevice->mDirectCommandList.Get(), indices.data(), ibByteSize, geo->IndexBufferUploader);
//
//	geo->VertexByteStride = sizeof(Vertex);
//	geo->VertexBufferByteSize = vbByteSize;
//	geo->IndexFormat = DXGI_FORMAT_R16_UINT;
//	geo->IndexBufferByteSize = ibByteSize;
//
//	geo->DrawArgs["box"] = boxSubmesh;
//
//	mGeometries[geo->Name] = std::move(geo);*/
//}
//
//void CrateApp::BuildPSOs()
//{
//
//
//
//	/*RenderContextGenerator gen(mDevice.get());
//	gen.CreateDefault();
//	mRenderContext = gen.GetRenderContext();
//
//	
//
//
//	int id = ShaderProperty::ProprtyToID("gDiffuseAlbedo");
//	mRenderContext->mShaderProperties->SetValue<XMFLOAT4>(id, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), ShaderValueType::Float4);
//	id = ShaderProperty::ProprtyToID("gFresnelR0");
//	mRenderContext->mShaderProperties->SetValue<XMFLOAT3>(id, XMFLOAT3(0.05f, 0.05f, 0.05f), ShaderValueType::Float4);
//   id = ShaderProperty::ProprtyToID("gRoughness");
//	mRenderContext->mShaderProperties->SetValue<float>(id, 0.2f, ShaderValueType::Float);*/
//
//
//}
//
//void CrateApp::BuildFrameResources()
//{
//    for(int i = 0; i < gNumFrameResources; ++i)
//    {
//
//        mFrameResources.push_back(std::make_unique<FrameResource>(md3dDevice.Get(),
//            1, (UINT)mAllRitems.size(), 0));
//    }
//}
//
//
//
//void CrateApp::BuildRenderItems()
//{
//	auto boxRitem = std::make_unique<RenderItem>();
//	boxRitem->ObjCBIndex = 0;
//	XMStoreFloat4x4(&boxRitem->World, XMMatrixScaling(2.0f, 2.0f, 2.0f) * XMMatrixTranslation(0.0f, 1.0f, 0.0f));
//
//	boxRitem->Geo = mGeometries["boxGeo"].get();
//	boxRitem->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
//	boxRitem->IndexCount = boxRitem->Geo->DrawArgs["box"].IndexCount;
//	boxRitem->StartIndexLocation = boxRitem->Geo->DrawArgs["box"].StartIndexLocation;
//	boxRitem->BaseVertexLocation = boxRitem->Geo->DrawArgs["box"].BaseVertexLocation;
//	mAllRitems.push_back(std::move(boxRitem));
//
//	// All the render items are opaque.
//	for(auto& e : mAllRitems)
//		mOpaqueRitems.push_back(e.get());
//}
//
//void CrateApp::DrawRenderItems(ID3D12GraphicsCommandList* cmdList, const std::vector<RenderItem*>& ritems)
//{
//    UINT objCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(ObjectConstants));
//  
// 
//	auto objectCB = mCurrFrameResource->ObjectCB->Resource();
//
//    // For each render item...
//    for(size_t i = 0; i < ritems.size(); ++i)
//    {
//        auto ri = ritems[i];
//
//        cmdList->IASetVertexBuffers(0, 1, &ri->Geo->VertexBufferView());
//        cmdList->IASetIndexBuffer(&ri->Geo->IndexBufferView());
//        cmdList->IASetPrimitiveTopology(ri->PrimitiveType);
//
//	
//
//        D3D12_GPU_VIRTUAL_ADDRESS objCBAddress = objectCB->GetGPUVirtualAddress() + ri->ObjCBIndex*objCBByteSize;
//
//		/*cmdList->SetGraphicsRootDescriptorTable(0, mSrv->GetGPUView());
//        cmdList->SetGraphicsRootConstantBufferView(1, objCBAddress);
//		auto address = mRenderContext->mShaderProperties->mUploadGraphicsData->Resource->GetGPUVirtualAddress();*/
//		//cmdList->SetGraphicsRootConstantBufferView(3, address);
//        cmdList->DrawIndexedInstanced(ri->IndexCount, 1, ri->StartIndexLocation, ri->BaseVertexLocation, 0);
//    }
//}
//
//std::array<const CD3DX12_STATIC_SAMPLER_DESC, 6> CrateApp::GetStaticSamplers()
//{
//	// Applications usually only need a handful of samplers.  So just define them all up front
//	// and keep them available as part of the root signature.  
//
//	const CD3DX12_STATIC_SAMPLER_DESC pointWrap(
//		0, // shaderRegister
//		D3D12_FILTER_MIN_MAG_MIP_POINT, // filter
//		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
//		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
//		D3D12_TEXTURE_ADDRESS_MODE_WRAP); // addressW
//
//	const CD3DX12_STATIC_SAMPLER_DESC pointClamp(
//		1, // shaderRegister
//		D3D12_FILTER_MIN_MAG_MIP_POINT, // filter
//		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
//		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
//		D3D12_TEXTURE_ADDRESS_MODE_CLAMP); // addressW
//
//	const CD3DX12_STATIC_SAMPLER_DESC linearWrap(
//		2, // shaderRegister
//		D3D12_FILTER_MIN_MAG_MIP_LINEAR, // filter
//		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
//		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
//		D3D12_TEXTURE_ADDRESS_MODE_WRAP); // addressW
//
//	const CD3DX12_STATIC_SAMPLER_DESC linearClamp(
//		3, // shaderRegister
//		D3D12_FILTER_MIN_MAG_MIP_LINEAR, // filter
//		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
//		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
//		D3D12_TEXTURE_ADDRESS_MODE_CLAMP); // addressW
//
//	const CD3DX12_STATIC_SAMPLER_DESC anisotropicWrap(
//		4, // shaderRegister
//		D3D12_FILTER_ANISOTROPIC, // filter
//		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
//		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
//		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressW
//		0.0f,                             // mipLODBias
//		8);                               // maxAnisotropy
//
//	const CD3DX12_STATIC_SAMPLER_DESC anisotropicClamp(
//		5, // shaderRegister
//		D3D12_FILTER_ANISOTROPIC, // filter
//		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
//		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
//		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressW
//		0.0f,                              // mipLODBias
//		8);                                // maxAnisotropy
//
//	return { 
//		pointWrap, pointClamp,
//		linearWrap, linearClamp, 
//		anisotropicWrap, anisotropicClamp };
//}
//
