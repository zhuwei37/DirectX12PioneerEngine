#include"GraphicsDevice.h"
#include"DescriptorHeapAllocator.h"



DescriptorHeapAllocator::DescriptorHeapAllocator(GraphicsDevice *GDevice
	,int DescriptorCount
	,D3D12_DESCRIPTOR_HEAP_TYPE Type, D3D12_DESCRIPTOR_HEAP_FLAGS flag):
	mDescriptorCount(DescriptorCount),
	mDescriptorType(Type),
	mGDevice(GDevice)
{
	
	D3D12_DESCRIPTOR_HEAP_DESC desc;
	desc.NumDescriptors = mDescriptorCount;
	desc.Flags = flag;
	desc.Type = mDescriptorType;
	desc.NodeMask = 0;
	ThrowGraphicsfFailed( GDevice->mD3dDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(mHeap.GetAddressOf())));
}
ID3D12DescriptorHeap* DescriptorHeapAllocator::GetNavigateDescriptorHeap()const
{
	return mHeap.Get();
}
int DescriptorHeapAllocator::GetDescriptorCount()const
{
	return mDescriptorCount;
}
Descriptor DescriptorHeapAllocator::Allocator(int Count)
{
	if (Count < 1)
	{
		return Descriptor{0};
	}
	int allocatorIndex = mCurrentUsedIndex + Count-1;
	if (allocatorIndex >= mDescriptorCount)
	{
		return Descriptor{ 0 };
	}
	
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuHandle =CD3DX12_CPU_DESCRIPTOR_HANDLE( mHeap->GetCPUDescriptorHandleForHeapStart());
	cpuHandle.Offset(mCurrentUsedIndex, mGDevice->CBV_SRV_UAV_DescriptorSize);

	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuHandle =CD3DX12_GPU_DESCRIPTOR_HANDLE( mHeap->GetGPUDescriptorHandleForHeapStart());
	gpuHandle.Offset(mCurrentUsedIndex, mGDevice->CBV_SRV_UAV_DescriptorSize);
	Descriptor descriptor = { cpuHandle,gpuHandle, Count };
	mCurrentUsedIndex += Count;
	return descriptor;
}