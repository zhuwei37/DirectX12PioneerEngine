#include"RenderSystem.h"
#include"DeferredRenderHost.h"
#include"RenderHostGroup.h"
RenderSystem::RenderSystem(GraphicsDevice* gDeivce):
	mGDevice(gDeivce),
	mRenderHostGroups(3)
{
	
	mRenderHostGroups[RenderHostType::MainRendering] = std::make_shared<RenderHostGroup>(RenderHostType::MainRendering);
	mRenderHostGroups[RenderHostType::MaterialRendering] = std::make_shared<RenderHostGroup>(RenderHostType::MaterialRendering);
	mRenderHostGroups[RenderHostType::AnimationRendering] = std::make_shared<RenderHostGroup>(RenderHostType::AnimationRendering);
}
RenderHost* RenderSystem::GetRenderHost(RenderHostId id)
{
	auto findIt = mRenderHostMap.find(id);
	if (findIt != mRenderHostMap.end())
	{
		return findIt->second.get();
	}
	return nullptr;
}

void RenderSystem::ActivateRenderer(Renderer* renderer, RenderHostType type)
{
	
	this->mRenderHostGroups[type]->AddRenderer(renderer);
	

}
void RenderSystem::CloseRenderer(Renderer* renderer, RenderHostType type)
{
	this->mRenderHostGroups[type]->RemoveRenderer(renderer);
}
RenderHostId RenderSystem::CreateRenderHost(CreateRenderHostParameter parameter)
{
	try
	{

		std::shared_ptr<RenderHost> renderHost = std::make_shared<DeferredRenderHost>(mGDevice, &parameter);
		RenderHostId id = GetNextRenderHostId();
		mRenderHostMap[id] = renderHost;
		this->mRenderHostGroups[parameter.RenderHostType]->AddRenderHost(renderHost.get());
		return id;
	}
	catch (GraphicsException ex)
	{
	}
	return 0;
}
void RenderSystem::DestoryRenderHost(RenderHostId id, RenderHostType type)
{
	auto findIt = mRenderHostMap.find(id);
	if (findIt != mRenderHostMap.end())
	{
		this->mRenderHostGroups[type]->RemoveRenderHost(findIt->second.get());
		mRenderHostMap.erase(findIt);
		
	}
}
void RenderSystem::PreRender()
{
	//触发RendererHost PreRender
	//mActiveRenderHosts.clear();
	for (auto renderHostGroup : this->mRenderHostGroups)
	{
		renderHostGroup->PreRender();
	}
}
void RenderSystem::OnRender()
{
	if (mRenderHostMap.size() == 0)
	{
		return;
	}

	std::vector< ID3D12CommandList*> commandListsCompleteds;
	int ComListIndex = 0;
	//优化存在一个活动列表记录活动RenderHost
	for (auto renderHostGroup : this->mRenderHostGroups)
	{
		
			auto CommandList_Completed = renderHostGroup->OnRender();
			for (int i=0;i< CommandList_Completed.size();i++)
			{
				if (CommandList_Completed[i] != nullptr)
				{
					commandListsCompleteds.push_back ( CommandList_Completed[i].Get());
				}
			}
		
	}
	
	//结束录制命令
	//ThrowGraphicsfFailed(commandList->Close());
	//执行录制命令
	if (commandListsCompleteds.size() > 0 )
	{
		
			
			mGDevice->ExctureCommands(commandListsCompleteds.data(), commandListsCompleteds.size());
	}
	//翻转显示
	for (auto renderHost : this->mRenderHostMap)
	{
		if (renderHost.second->IsVisible())
		{
			renderHost.second->Present();
		}
	}
	//等待
	mGDevice->FlushCommandQueue();
}
RenderHostId RenderSystem::GetNextRenderHostId()
{
	return RenderHostIdCounter++;
}