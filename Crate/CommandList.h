#pragma once
#include"wrl.h"
#include"GraphicsResource.h"
using namespace Microsoft::WRL;



class CommandList
{
public:
	CommandList(GraphicsDevice* graphicsDevice);
	//×ÊÔ´×´Ì¬×ª»»
	void ResourceBarrier(GraphicsResource* resource, D3D12_RESOURCE_STATES beforeState, D3D12_RESOURCE_STATES afterState);

	

private:
	ComPtr<ID3D12CommandList> mCommandList;
};