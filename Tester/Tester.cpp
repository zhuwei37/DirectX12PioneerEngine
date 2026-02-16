// Tester.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "Tester.h"
#include"CrateApp.h"
#include"SharedTexture.h"
#include<memory>
#include<iostream>
#include<d3d11.h>
#include<dxgi.h>
#include <d3d9.h>
#include"CameraController.h"
#pragma comment (lib,"d3d11.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment (lib,"Crate.lib")
#pragma comment(lib,"Entity.lib")

#include"Windows.h"
#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
int WinMain222();
HWND hwnd;
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
 
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此处放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, 0, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }
    WinMain222();
 /*   HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PROJECT1));*/

    MSG msg;
  
    // 主消息循环:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, 0, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(0, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName =L"ssss";
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // 将实例句柄存储在全局变量中

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }
    hwnd = hWnd;
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 分析菜单选择:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: 在此处添加使用 hdc 的任何绘图代码...
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

int APIENTRY WinMain222()
{


#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    try
    {
        CrateApp theApp;
        theApp.mClientHeight = 600;
        theApp.mClientWidth = 800;
        theApp.mHandle = hwnd;
        if (!theApp.Initialize())
            return 0;
        D3D_FEATURE_LEVEL featureLevels[] = {
    D3D_FEATURE_LEVEL_11_0,
 
        };
        ID3D11Device* device = nullptr;
        ID3D11DeviceContext* context = nullptr;
        ThrowIfFailed(D3D11CreateDevice(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            D3D11_CREATE_DEVICE_DEBUG,
            featureLevels,
            ARRAYSIZE(featureLevels),
            D3D11_SDK_VERSION,
            &device, nullptr, &context
        ));

      
       
      
        

       // D3D11_TEXTURE2D_DESC texDesc;
       // ZeroMemory(&texDesc, sizeof(texDesc));
       // texDesc.Width = 512;
       // texDesc.Height = 512;
       // texDesc.MipLevels = 1;
       // texDesc.ArraySize = 1;
       // texDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
       // texDesc.SampleDesc.Count = 1;
       // texDesc.Usage = D3D11_USAGE_DEFAULT;
       // texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
       // texDesc.MiscFlags = D3D11_RESOURCE_MISC_SHARED; // 共享标志
       // ID3D11Texture2D* g_pSharedTexture = nullptr;
       //auto hr = device->CreateTexture2D(&texDesc, nullptr, &g_pSharedTexture);

       //IDXGIResource* pDXGIResource = nullptr;
       //HANDLE g_hSharedHandle=nullptr;
       //hr = g_pSharedTexture->QueryInterface(IID_PPV_ARGS(&pDXGIResource));
       //if (SUCCEEDED(hr))
       //{
       //    hr = pDXGIResource->GetSharedHandle(&g_hSharedHandle);
       //    pDXGIResource->Release();
       //}

        ID3D11Device1* pD11Device1 = nullptr;
        IDXGIDevice1* dxgiDev = nullptr;
        HRESULT hr = device->QueryInterface(__uuidof(ID3D11Device1), (void**)&pD11Device1);
        hr = device->QueryInterface(__uuidof(IDXGIDevice1), (void**)&dxgiDev);
        if (SUCCEEDED(hr)) {
            // 2. 打开共享资源
            ID3D11Texture2D* pD11SharedResource = nullptr;
        
         
          
            if (SUCCEEDED(hr)) {
                
                D3D11_TEXTURE2D_DESC desc;
                pD11SharedResource->GetDesc(&desc);
                // 3. 转换为纹理接口（如果需要）
               IDXGIResource1* pD11SharedTexture = nullptr;
      
         
               HRESULT hr1 = 0;

                hr1 = pD11SharedResource->QueryInterface(__uuidof(IDXGIResource1), (void**)&pD11SharedTexture);
                HANDLE h=nullptr;
                HRESULT hr2 = 0;
                hr2=pD11SharedTexture->GetSharedHandle(&h);
                
                int a = 1;
                // ... 现在你可以在DX11的着色器中采样这个纹理了
            }
            pD11Device1->Release();
        }

       
    
       /* ID3D12Resource* r;*/
    /*   auto hr= theApp.md3dDevice->OpenSharedHandle
        (handle, IID_PPV_ARGS(&r));*/
       /* theApp.md3dDevice->OpenSharedHandle(mSharedTexture->GetSharedTextureHandle(),
            )*/
    /*   auto desc= r->GetDesc();*/
     /*   timer.Tick();
        theApp.Update(timer);
        Sleep(200);
        theApp.Draw(mSharedTexture.get());
        theApp.Update(timer);
        Sleep(200);
        theApp.Update(timer);
        theApp.Draw(mSharedTexture.get());
        Sleep(200);*/
        return 1;
    }
    catch (DxException& e)
    {
        MessageBox(nullptr, e.ToString().c_str(), L"HR Failed", MB_OK);
        return 0;
    }

}



