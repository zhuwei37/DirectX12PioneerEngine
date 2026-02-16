#include"GraphicsDevice.h"
#include"CommandQueue.h"
#include"ExceptionEx.h"
#include"vector"

CommandQueue::CommandQueue(GraphicsDevice* device):
	mGDevice(device)
{
	Init();
}
void CommandQueue::Init()
{
	ThrowGraphicsfFailed(mGDevice->mD3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE,
		IID_PPV_ARGS(&mFence)));

	D3D12_COMMAND_QUEUE_DESC desc = {};
	desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	ThrowGraphicsfFailed(mGDevice->mD3dDevice->CreateCommandQueue
	(
		&desc, IID_PPV_ARGS(&mDXCommandQueue)
	));

}
void CommandQueue::FlushCommandQueue()
{
	mCurrentFence++;
	ThrowGraphicsfFailed(mDXCommandQueue->Signal(mFence.Get(), mCurrentFence));

	if (mFence->GetCompletedValue() < mCurrentFence)
	{
		HANDLE eventHandle = CreateEventEx(nullptr, false, false, EVENT_ALL_ACCESS);

		ThrowGraphicsfFailed(mFence->SetEventOnCompletion(mCurrentFence, eventHandle));

		WaitForSingleObject(eventHandle, INFINITE);
		CloseHandle(eventHandle);
	}
}
void CommandQueue::Execute(ID3D12CommandList* cmdLists[],int cmdListCount)
{
	/*std::vector<ID3D12CommandList*> cmds(cmdListCount);
	for (int i = 0; i < cmdListCount; i++)
	{
		cmds[i]=
	}*/
	mDXCommandQueue->ExecuteCommandLists(cmdListCount, cmdLists);
}
void  CommandQueue::Signal()
{
	mCurrentFence++;
	mDXCommandQueue->Signal(mFence.Get(), mCurrentFence);
}