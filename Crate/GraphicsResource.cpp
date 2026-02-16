#include"GraphicsResource.h"
#include"d3dUtil.h"
#include"GraphicsDevice.h"
ID3D12Resource* GraphicsResource::NavigateResource()
{
	if (Resource != nullptr)
	{
		return Resource.Get();
	}
	return nullptr;
}
BYTE* GraphicsResource::Map()
{
	if (Resource != nullptr)
	{
		void* p;
		auto hr=Resource->Map(0, nullptr,&p);
		return (BYTE*)p;
	}
	return nullptr;
}
void GraphicsResource::UnMap()
{
	if (Resource != nullptr)
	{
		Resource->Unmap(0, nullptr);
	}
}
GraphicsResource* GraphicsResource::CreateBuffer(GraphicsDevice* GDevcie, UINT buffersize, D3D12_HEAP_TYPE heap_type, D3D12_RESOURCE_STATES state)
{
	ComPtr<ID3D12Resource> res;
	CD3DX12_HEAP_PROPERTIES property(heap_type);
	auto desc=CD3DX12_RESOURCE_DESC::Buffer(buffersize);
	auto hr = GDevcie->mD3dDevice->CreateCommittedResource
	(
		&property,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		state,
		nullptr,
		IID_PPV_ARGS(res.GetAddressOf())
		 
	);
	ThrowGraphicsfFailed(hr);
	GraphicsResource* newResource = new GraphicsResource(res);
	return newResource;
}
GraphicsResource* GraphicsResource::CreateBuffer(GraphicsDevice* gDevice, UINT buffersize, BYTE* data, D3D12_HEAP_TYPE heap_type, D3D12_RESOURCE_STATES state)
{

	
	gDevice->FlushCommandQueue();
	gDevice->ResetDirectCommandAlloc();
	gDevice->ResetDirectCommandList();
	auto resource = CreateBuffer(gDevice, buffersize, heap_type, state);
	auto tempResource = CreateBuffer(gDevice, buffersize, D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ);
	auto data1 = tempResource->Map();
	memcpy(data1, data, buffersize);
	tempResource->UnMap();
	auto before=CD3DX12_RESOURCE_BARRIER::Transition(resource->Resource.Get(), state, D3D12_RESOURCE_STATE_COPY_DEST);
	auto after = CD3DX12_RESOURCE_BARRIER::Transition(resource->Resource.Get(), D3D12_RESOURCE_STATE_COPY_DEST, state);
	//auto b2 = CD3DX12_RESOURCE_BARRIER::Transition(tempResource->Resource.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_SOURCE);
	//gDevice->mDirectCommandList->ResourceBarrier(1, &b2);
	gDevice->mDirectCommandList->ResourceBarrier(1, &before);
	gDevice->mDirectCommandList->CopyBufferRegion(resource->NavigateResource(), 0, tempResource->NavigateResource(), 0, buffersize);
	gDevice->mDirectCommandList->ResourceBarrier(1, &after);
	gDevice->ExctureCommands();
	gDevice->FlushCommandQueue();
	delete tempResource;
	return resource;
}
