#pragma once
#include"GraphicsDevice.h"
#include"GraphicsException.h"
#include"ExceptionEx.h"
#include "wrl.h"
#include"d3dx12.h"
#include"d3d12.h"
#pragma comment(lib, "D3D12.lib")
using namespace Microsoft::WRL;
class SwapChainGraphicsPresenter
{
public:
	SwapChainGraphicsPresenter( GraphicsDevice* graphicsDevice,HWND hwnd,UINT width,UINT height);
	void Init(HWND winHandle,UINT width, UINT height);
	void Resize(UINT width, UINT height);
	void Present();
	const UINT SwapChainBufferCount = 2;
	ID3D12Resource* GetCurrentBuffer();
	D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentBufferView()const;

private:
	void CreateAndBuildRenderTargetView();
	void CreateRenderTargetViewDesciptorHeap();
	ComPtr<IDXGISwapChain> mSwapChain;
	ComPtr<ID3D12Resource> mSwapChainBuffer[2];
	ComPtr<ID3D12DescriptorHeap> mRtvHeap;
	GraphicsDevice* graphicsDevice;
	HANDLE mhWnd;
	UINT mWidth;
	UINT mHeight;
	UINT mCurrBackBuffer = 0;


};