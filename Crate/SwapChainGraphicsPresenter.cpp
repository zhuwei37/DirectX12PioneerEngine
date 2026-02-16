#include"SwapChainGraphicsPresenter.h"



SwapChainGraphicsPresenter::SwapChainGraphicsPresenter(GraphicsDevice* graphicsDevice, HWND hwnd, UINT width, UINT height)
	:mhWnd(hwnd),
	graphicsDevice(graphicsDevice),
	mWidth(width),
	mHeight(height)
{
	Init(hwnd, mWidth, mHeight);
}
void SwapChainGraphicsPresenter::Init(HWND hwnd,UINT width,UINT height)
{
	mSwapChain.Reset();
	DXGI_SWAP_CHAIN_DESC desc;
	desc.BufferDesc.Width = width;
	desc.BufferDesc.Height = height;
	desc.BufferDesc.RefreshRate.Denominator = 1;
	desc.BufferDesc.RefreshRate.Numerator = 60;
	desc.BufferDesc.Format = graphicsDevice->BackBufferFormat;
	desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	desc.BufferDesc.Scaling = DXGI_MODE_SCALING_STRETCHED;

	desc.BufferCount = SwapChainBufferCount;

	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;

	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	desc.OutputWindow = hwnd;
	desc.Windowed = true;
	desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	graphicsDevice->mDxgiFactory4->CreateSwapChain(
		graphicsDevice->DirectCommandQueue->mDXCommandQueue.Get(),
		&desc,
		this->mSwapChain.GetAddressOf()
	);
	CreateRenderTargetViewDesciptorHeap();
	CreateAndBuildRenderTargetView();
}
D3D12_CPU_DESCRIPTOR_HANDLE SwapChainGraphicsPresenter::GetCurrentBufferView() const
{
	return CD3DX12_CPU_DESCRIPTOR_HANDLE(
		mRtvHeap->GetCPUDescriptorHandleForHeapStart(),
		mCurrBackBuffer,
		graphicsDevice->GetDescriptorHeapIncrementSize(DESCRIPTOR_HEAP_TYPE::RTV)
	);
}
void SwapChainGraphicsPresenter::Resize(UINT width,UINT height)
{
	this->mWidth = width;
	this->mHeight = height;
	for (int i = 0; i < SwapChainBufferCount; i++)
	{
		mSwapChainBuffer[i].Reset();
	}
	ThrowGraphicsfFailed(mSwapChain->ResizeBuffers(
		SwapChainBufferCount,
		mWidth,
		mHeight,
		graphicsDevice->BackBufferFormat,
		DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
	));
	CreateAndBuildRenderTargetView();
	mCurrBackBuffer = 0;
}
void SwapChainGraphicsPresenter::CreateAndBuildRenderTargetView()
{
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(mRtvHeap->GetCPUDescriptorHandleForHeapStart());
	for (UINT i = 0; i < SwapChainBufferCount; i++)
	{
		ThrowGraphicsfFailed(mSwapChain->GetBuffer(i, IID_PPV_ARGS(&mSwapChainBuffer[i])));
		graphicsDevice->mD3dDevice->CreateRenderTargetView(mSwapChainBuffer[i].Get(), nullptr, rtvHeapHandle);
		rtvHeapHandle.Offset(1, graphicsDevice->GetDescriptorHeapIncrementSize(DESCRIPTOR_HEAP_TYPE::RTV));
	}
}
void SwapChainGraphicsPresenter::Present()
{
	ThrowGraphicsfFailed(mSwapChain->Present(0, 0));
	mCurrBackBuffer = (mCurrBackBuffer + 1) % SwapChainBufferCount;
}
ID3D12Resource* SwapChainGraphicsPresenter::GetCurrentBuffer()
{
	return mSwapChainBuffer[mCurrBackBuffer].Get();
}
void SwapChainGraphicsPresenter::CreateRenderTargetViewDesciptorHeap()
{
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc;
	rtvHeapDesc.NumDescriptors = SwapChainBufferCount;
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.NodeMask = 0;
	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	ThrowGraphicsfFailed(
		graphicsDevice->mD3dDevice->CreateDescriptorHeap(
			&rtvHeapDesc, IID_PPV_ARGS(&mRtvHeap)
		));
}