//***************************************************************************************
// d3dApp.h by Frank Luna (C) 2015 All Rights Reserved.
//***************************************************************************************

#pragma once

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#include "d3dUtil.h"
#include"ExceptionEx.h"
#include"GraphicsDevice.h"
#include"SwapChainGraphicsPresenter.h"
#include"memory.h"
#include"GTexture.h"
#include"DepthStencilResourceView.h"
// Link necessary d3d12 libraries.
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib,"Common.lib")
class D3DApp
{
protected:

    D3DApp();
    D3DApp(const D3DApp& rhs) = delete;
    D3DApp& operator=(const D3DApp& rhs) = delete;
    virtual ~D3DApp();

public:


    Microsoft::WRL::ComPtr<ID3D12Device> md3dDevice;

    virtual void OnResize(int width, int height);
    virtual bool Initialize();
    int mClientWidth = 800;
    int mClientHeight = 600;
    HWND mHandle;
protected:
    bool InitDirect3D();
   

    void FlushCommandQueue();

 


protected:

 
    
    std::unique_ptr<GraphicsDevice> mDevice;
    std::unique_ptr<SwapChainGraphicsPresenter> Presenter;
    std::shared_ptr< DepthStencilResourceView> mDsv;

  
  //  Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mCommandList;


 

    D3D12_VIEWPORT mScreenViewport;
    D3D12_RECT mScissorRect;


    D3D_DRIVER_TYPE md3dDriverType = D3D_DRIVER_TYPE_HARDWARE;
    DXGI_FORMAT mBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
    DXGI_FORMAT mDepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;




};

