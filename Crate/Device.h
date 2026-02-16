#pragma once
#include <dxgi1_4.h>
#include<d3d12.h>
#include"ExceptionEx.h"
#include<wrl.h>
#pragma comment (lib,"dxgi.lib")
using namespace Microsoft::WRL;
class Device
{
public:
	ComPtr<ID3D12Device> mDX12Device;
	ComPtr<IDXGIFactory4> mDxgiFactory4;

	Device()
	{
		Init();
	}
	void Init()
	{
#if defined(DEBUG) || defined(_DEBUG) 
		{
			ComPtr<ID3D12Debug> debugController;
			ThrowGraphicsfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)));
			debugController->EnableDebugLayer();
			ComPtr<ID3D12DebugDevice> debugDevice;
			if (SUCCEEDED(debugController.As(&debugDevice)))
			{
				debugDevice->ReportLiveDeviceObjects(D3D12_RLDO_SUMMARY | D3D12_RLDO_DETAIL);
			}
		}
#endif
	ThrowGraphicsfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&mDxgiFactory4)));
	HRESULT hardwareResult = D3D12CreateDevice(
		nullptr,
		D3D_FEATURE_LEVEL_11_0,
		IID_PPV_ARGS(&mDX12Device)
	);
	if (FAILED(hardwareResult))
	{
		ComPtr<IDXGIAdapter> pWarpAdapter;
		ThrowGraphicsfFailed(mDxgiFactory4->EnumWarpAdapter(IID_PPV_ARGS(&pWarpAdapter)));
		ThrowGraphicsfFailed(D3D12CreateDevice(
			pWarpAdapter.Get(),
			D3D_FEATURE_LEVEL_11_0,
			IID_PPV_ARGS(&mDX12Device)
		));
	}
	}
public:
	ID3D12Device* GetDevice()
	{
		return mDX12Device.Get();
	}
	IDXGIFactory4* GetFactory4()
	{
		return mDxgiFactory4.Get();
	}

	static Device* Get()
	{
		static Device dev;

		return &dev;


	}
};