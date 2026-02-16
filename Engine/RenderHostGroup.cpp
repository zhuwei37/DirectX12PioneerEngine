#include"RenderHostGroup.h"

#include"RenderSystem.h"
RenderHostGroup::RenderHostGroup(RenderSystem::RenderHostType type)
{
	this->mRenderHostGroupType = type;
}

void RenderHostGroup::AddRenderer(Renderer* renderer)
{
	if (renderer == nullptr)
	{
		return;
	}
	Layer layer = renderer->GetLayer();
	/*mRenderHostList.push_back(renderer)*/
	auto findItem = this->mRendererMap.find(layer);
	if (findItem != this->mRendererMap.end())
	{
		findItem->second.push_back(renderer);
	}
	else
	{
		this->mRendererMap[layer] = RendererList();
		this->mRendererMap[layer].push_back(renderer);

	}
}

void RenderHostGroup::RemoveRenderer(Renderer* renderer)
{
	if (renderer == nullptr)
	{
		return;
	}
	Layer layer = renderer->GetLayer();
	auto findItem = this->mRendererMap.find(layer);
	if (findItem != this->mRendererMap.end())
	{
		auto removeItem=std::remove(
			this->mRendererMap[layer].begin(),
			this->mRendererMap[layer].end(),
			renderer);
		if (removeItem != this->mRendererMap[layer].end())
		{
			this->mRendererMap[layer].erase(removeItem);
		}
	}
}

void RenderHostGroup::AddRenderHost(RenderHost* renderHost)
{
	this->mRenderHostList.push_back(renderHost);
}
void RenderHostGroup::RemoveRenderHost(RenderHost* renderHost)
{
	auto removeItem=std::remove(this->mRenderHostList.begin(), this->mRenderHostList.end(), renderHost);
	if (removeItem != this->mRenderHostList.end())
	{
		this->mRenderHostList.erase(removeItem);
	}
}
void RenderHostGroup::PreRender()
{
	mActiveRenderHosts.clear();
	for (auto renderHost : this->mRenderHostList)
	{
		if(renderHost->IsVisible())
		{
			mActiveRenderHosts.push_back(renderHost);
		}
	}
	for (auto renderHost : this->mActiveRenderHosts)
	{
		renderHost->PreRender(this->mRendererMap,&this->mSkyBox);
	}
}

std::vector<ComPtr< ID3D12GraphicsCommandList>> RenderHostGroup::OnRender()
{
	std::vector<ComPtr< ID3D12GraphicsCommandList>> cmdLists(this->mActiveRenderHosts.size());
	for (int i = 0; i < this->mActiveRenderHosts.size(); i++)
	{
		auto cmdList = mActiveRenderHosts[i]->OnRender();
		cmdLists[i]=(cmdList);
	}

	return cmdLists;
}

RenderSystem::RenderHostType RenderHostGroup::GetRenderHostType()
{
	return this->mRenderHostGroupType;
}