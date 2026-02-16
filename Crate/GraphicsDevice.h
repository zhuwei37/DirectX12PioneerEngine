#pragma once
#include"d3dx12.h"
#include"wrl.h"
#include"dxgi1_4.h"
#include"Device.h"
#include<memory.h>
#include"CommandQueue.h"
#include"ExceptionEx.h"
#include"DescriptorHeapAllocator.h"
using namespace Microsoft::WRL;

enum DESCRIPTOR_HEAP_TYPE
{
	Sampler,
	RTV,
	DSV,
	CBV_SRV_UAV,
	Count,
};
class GraphicsDevice
{
public:

	static GraphicsDevice *Get()
	{
		
		static GraphicsDevice instance;
		return &instance;
	}
	GraphicsDevice();
	
	void LogAdapters()
	{
		UINT i = 0;
		IDXGIAdapter* adapter = nullptr;
		std::vector<IDXGIAdapter*> adapterList;
		while (mDxgiFactory4->EnumAdapters(i, &adapter) != DXGI_ERROR_NOT_FOUND)
		{
			DXGI_ADAPTER_DESC desc;
			adapter->GetDesc(&desc);

			std::wstring text = L"***Adapter: ";
			text += desc.Description;
			text += L"\n";

		//	OutputDebugString(text.c_str());

			adapterList.push_back(adapter);

			++i;
		}

		for (size_t i = 0; i < adapterList.size(); ++i)
		{
			LogAdapterOutputs(adapterList[i]);
			ReleaseCom(adapterList[i]);
		}
	}

	void LogAdapterOutputs(IDXGIAdapter* adapter)
	{
		UINT i = 0;
		IDXGIOutput* output = nullptr;
		while (adapter->EnumOutputs(i, &output) != DXGI_ERROR_NOT_FOUND)
		{
			DXGI_OUTPUT_DESC desc;
			output->GetDesc(&desc);

			std::wstring text = L"***Output: ";
			text += desc.DeviceName;
			text += L"\n";
		//	OutputDebugString(text.c_str());

			LogOutputDisplayModes(output, BackBufferFormat);

			ReleaseCom(output);

			++i;
		}
	}

	void LogOutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format)
	{
		UINT count = 0;
		UINT flags = 0;

		// Call with nullptr to get list count.
		output->GetDisplayModeList(format, flags, &count, nullptr);

		std::vector<DXGI_MODE_DESC> modeList(count);
		output->GetDisplayModeList(format, flags, &count, &modeList[0]);

		for (auto& x : modeList)
		{
			UINT n = x.RefreshRate.Numerator;
			UINT d = x.RefreshRate.Denominator;
			std::wstring text =
				L"Width = " + std::to_wstring(x.Width) + L" " +
				L"Height = " + std::to_wstring(x.Height) + L" " +
				L"Refresh = " + std::to_wstring(n) + L"/" + std::to_wstring(d) +
				L"\n";

			//::OutputDebugString(text.c_str());
		}
	}
	ComPtr<ID3D12Device> mD3dDevice;
	ComPtr<IDXGIFactory4> mDxgiFactory4;

	std::unique_ptr<CommandQueue> DirectCommandQueue;
	ComPtr<ID3D12GraphicsCommandList> mDirectCommandList;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> mDirectCmdListAlloc;

	UINT GetDescriptorHeapIncrementSize(DESCRIPTOR_HEAP_TYPE type);
	std::unique_ptr<DescriptorHeapAllocator> CBV_SRV_UAV_No_Shader_Visible_DescriptorHeapAllocator;
	std::unique_ptr<DescriptorHeapAllocator> CBV_SRV_UAV_Shader_Visible_DescriptorHeapAllocator;
	std::unique_ptr<DescriptorHeapAllocator> DsvDescriptorHeapAllocator;
	
	/// <summary>
	/// RTV ¶ÑÃèÊö·û
	/// </summary>
	std::unique_ptr<DescriptorHeapAllocator> RtvDescriptorHeapAllocator;
	int CBV_SRV_UAV_DescriptorSize;
	int RTV_DescriptorSize;

	void ResetDirectCommandList();
	void ResetDirectCommandAlloc();
	void FlushCommandQueue();
	void ExctureCommands();
	void ExctureCommands(ID3D12CommandList** cmdList, int Count);


	void BeginCommand();
	void EndCommand();

public:

	void CreateCommandObjects();


	const DXGI_FORMAT BackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	const DXGI_FORMAT DepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

};