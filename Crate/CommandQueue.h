#pragma once

#include"CommandList.h"
class GraphicsDevice;
class CommandQueue
{
public:
	CommandQueue(GraphicsDevice* device);
	void FlushCommandQueue();
	void Execute(ID3D12CommandList* cmdLists[], int cmdListCount=1 );
	void Signal();
	ComPtr<ID3D12CommandQueue> mDXCommandQueue;
private:
	void Init();
	ComPtr<ID3D12Fence> mFence;
	GraphicsDevice* mGDevice;
	unsigned long mCurrentFence=0;

};