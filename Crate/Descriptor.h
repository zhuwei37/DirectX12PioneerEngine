#pragma once
#include"d3d12.h"
typedef struct
{
	D3D12_CPU_DESCRIPTOR_HANDLE CpuHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE GpuHandle;
	int Number;
} Descriptor;