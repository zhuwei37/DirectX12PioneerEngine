#pragma once
#include"d3dx12.h"
#include"wrl.h"
using namespace Microsoft::WRL;
class GraphicsDevice;
class GraphicsResource
{
	
public :
	GraphicsResource() {};
	GraphicsResource(ComPtr<ID3D12Resource> res) 
	{
		this->Resource = res;
	}
	BYTE* Map();
	void UnMap();
	/*GraphicsResource()=delete;*/
    ID3D12Resource* NavigateResource();
	
	static GraphicsResource* CreateBuffer(GraphicsDevice* GDevcie, UINT buffersize, D3D12_HEAP_TYPE heap_type,D3D12_RESOURCE_STATES state);
	static GraphicsResource* CreateBuffer(GraphicsDevice* GDevice, UINT bufferSize, BYTE* data, D3D12_HEAP_TYPE heap_type, D3D12_RESOURCE_STATES state);
	~GraphicsResource()
	{
		if (Resource != nullptr)
		{
			Resource = nullptr;
		}
	}
public:
	ComPtr<ID3D12Resource> Resource;
	

};