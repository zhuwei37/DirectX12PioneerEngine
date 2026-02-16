#pragma once
#include"d3dUtil.h"
#include"Descriptor.h"
class GraphicsDevice;

class DescriptorHeapAllocator
{
public:
	DescriptorHeapAllocator(GraphicsDevice* GDevice, int DescriptorCount,
		D3D12_DESCRIPTOR_HEAP_TYPE Type, D3D12_DESCRIPTOR_HEAP_FLAGS flag= D3D12_DESCRIPTOR_HEAP_FLAG_NONE);
	Descriptor Allocator(int Count);
	ID3D12DescriptorHeap* GetNavigateDescriptorHeap()const;
	int GetDescriptorCount()const;
private:
	GraphicsDevice* mGDevice;
	int mCurrentUsedIndex = 0;
	int mDescriptorCount;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mHeap;
	D3D12_DESCRIPTOR_HEAP_TYPE mDescriptorType;
};