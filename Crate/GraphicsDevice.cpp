#include"GraphicsDevice.h"

GraphicsDevice::GraphicsDevice()
{
	Device* device = Device::Get();
	this->mD3dDevice = device->mDX12Device;
	this->mDxgiFactory4 = device->mDxgiFactory4;
	DirectCommandQueue = std::make_unique<CommandQueue>(this);
	CBV_SRV_UAV_DescriptorSize = GetDescriptorHeapIncrementSize(DESCRIPTOR_HEAP_TYPE::CBV_SRV_UAV);
	RTV_DescriptorSize = GetDescriptorHeapIncrementSize(DESCRIPTOR_HEAP_TYPE::RTV);
	CBV_SRV_UAV_No_Shader_Visible_DescriptorHeapAllocator = std::make_unique<DescriptorHeapAllocator>(this, 4096,D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV , D3D12_DESCRIPTOR_HEAP_FLAG_NONE);
	CBV_SRV_UAV_Shader_Visible_DescriptorHeapAllocator = std::make_unique<DescriptorHeapAllocator>(this, 4096*4, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);
	DsvDescriptorHeapAllocator = std::make_unique<DescriptorHeapAllocator>(this, 64, D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	RtvDescriptorHeapAllocator = std::make_unique<DescriptorHeapAllocator>(this, 64, D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQualityLevels;
	msQualityLevels.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	msQualityLevels.SampleCount = 4;
	msQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	msQualityLevels.NumQualityLevels = 0;
	ThrowGraphicsfFailed(mD3dDevice->CheckFeatureSupport(
		D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
		&msQualityLevels,
		sizeof(msQualityLevels)));
#ifdef _DEBUG
	LogAdapters();
#endif // DEBUG
	CreateCommandObjects();

}
UINT GraphicsDevice::GetDescriptorHeapIncrementSize(DESCRIPTOR_HEAP_TYPE type)
{
	switch (type)
	{
	case DESCRIPTOR_HEAP_TYPE::CBV_SRV_UAV:
	{
		return mD3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	}
	case DESCRIPTOR_HEAP_TYPE::RTV:
	{
		return mD3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	}
	case DESCRIPTOR_HEAP_TYPE::DSV:
	{
		return mD3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	}
	case DESCRIPTOR_HEAP_TYPE::Sampler:
	{
		return mD3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);
	}
	default:
	{
		return 0;
	}
	}
}

void GraphicsDevice::CreateCommandObjects()
{
	ThrowGraphicsfFailed(mD3dDevice->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(mDirectCmdListAlloc.GetAddressOf())
	));
	ThrowGraphicsfFailed(mD3dDevice->CreateCommandList
	(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		mDirectCmdListAlloc.Get(),
		nullptr,
		IID_PPV_ARGS(mDirectCommandList.GetAddressOf())
	));
	mDirectCommandList->Close();
}
void GraphicsDevice::ResetDirectCommandAlloc()
{
	ThrowGraphicsfFailed(mDirectCmdListAlloc->Reset());
}
void GraphicsDevice::ResetDirectCommandList()
{
	ThrowGraphicsfFailed(mDirectCommandList->Reset(mDirectCmdListAlloc.Get(),nullptr));
}
void GraphicsDevice::FlushCommandQueue()
{
	DirectCommandQueue->FlushCommandQueue();
}
void GraphicsDevice::ExctureCommands()
{
	ThrowGraphicsfFailed( mDirectCommandList->Close());
	ID3D12CommandList* cmdList[] = { mDirectCommandList.Get() };
	DirectCommandQueue->Execute(cmdList);
}
void GraphicsDevice::ExctureCommands(ID3D12CommandList** cmdList, int Count)
{
	DirectCommandQueue->Execute(cmdList,Count);
}
void GraphicsDevice::BeginCommand()
{
	this->FlushCommandQueue();
	this->ResetDirectCommandAlloc();
	this->ResetDirectCommandList();
}
void GraphicsDevice::EndCommand()
{
	this->ExctureCommands();
	this->FlushCommandQueue();
}