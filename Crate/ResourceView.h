#pragma once
#include<memory>
#include"GraphicsResource.h"
class ResourceView
{
public:
	
	
	virtual D3D12_CPU_DESCRIPTOR_HANDLE GetCPUView()const = 0;

	virtual D3D12_GPU_DESCRIPTOR_HANDLE GetGPUView()const = 0;
};