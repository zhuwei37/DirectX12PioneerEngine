#include"ResourceManager.h"
#include"interface_resource.h"
#include"MaterialResource.h"
ResourceManager::ResourceManager()
{

}
FolderResource* ResourceManager::GetRootFolderResource()const
{
	return mRootFolderResource;
}
void ResourceManager::SetRootFolderResource(FolderResource* rootFolder)
{
	mRootFolderResource = rootFolder;
}
std::shared_ptr<IResource> ResourceManager::GetResourceStrongPtr(ResourceID& id)
{
	auto findIt = mResourceMap.find(id);
	if (findIt != mResourceMap.end())
	{

		return findIt->second;
	}
	return nullptr;
}
std::weak_ptr<IResource> ResourceManager::GetResourceWeakPtr(ResourceID& id)
{
	auto findIt = mResourceMap.find(id);
	if (findIt != mResourceMap.end())
	{
		
		return  std::weak_ptr<IResource>( findIt->second);
	}
	return std::weak_ptr<IResource>();
}
IResource* ResourceManager::GetResource(ResourceID& id)
{
	auto findIt = mResourceMap.find(id);
	if (findIt != mResourceMap.end())
	{
		return findIt->second.get();;
	}
	return nullptr;
}

void ResourceManager::AddResource(std::shared_ptr<IResource> resource)
{
	
	mResourceMap[resource->GetId()]= resource;
	if (resource->IsInMemory())
	{
		mResourceInMemoryMap[resource->mId] = resource;
	}
}
bool ResourceManager::DeleteResouce(ResourceID& resourceId)
{
	auto findItem = mResourceMap.find(resourceId);
	if (findItem != mResourceMap.end())
	{
		mResourceMap.erase(findItem);
	}
	return true;
}

void ResourceManager::Save()
{
	
}
int ResourceManager::GetInMemeryResourceCount()
{
	return this->mResourceInMemoryMap.size();
}
std::vector<ResourceID> ResourceManager::GetInMemeryResourceIDs()
{
	std::vector<ResourceID> ids;
	for (auto& item : mResourceInMemoryMap)
	{
		ids.push_back( item.first);
	}
	return ids;
}
