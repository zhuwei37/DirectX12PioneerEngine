
#include"ExportResource.h"
#include"EngineManager.h"
ResourceInfo GetRootResourceInfo()
{
	
	ResourceSystem* resourceSystem = EngineManager::Get()->GetResourceSystem();

	auto resource=resourceSystem->GetResourceManager()->GetRootFolderResource();
	auto folder = dynamic_cast<FolderResource*>(resource);
	if (folder != nullptr)
	{
		ResourceInfo res;
		res.guid = folder->GetId();
		res.resourceType = folder->GetResourceType();
		res.childrenCount = folder->GetChildResources().size();
		res.hasChildren = res.childrenCount > 0;
		res.path = folder->GetPath().c_str();
		return res;
	}
	return {};
}

bool GetChildResourceInfo(ResourceID parentId,ResourceInfo* ChildInfos, int& size)
{
	ResourceSystem* resourceSystem = EngineManager::Get()->GetResourceSystem();
	auto manager = resourceSystem->GetResourceManager();
	auto parentResource = manager->GetResource(parentId);
	auto parentFolder = dynamic_cast<FolderResource*>(parentResource);
	if (parentFolder != nullptr)
	{
		
		auto childs = parentFolder->GetChildResources();
		if (childs.size() <= size)
		{
			size = childs.size();
			memset(ChildInfos, 0, sizeof(ResourceInfo) * size);
			int index = 0;
			for (auto& res : childs)
			{
				auto resource = manager->GetResource(res);
				if (resource != nullptr)
				{
					ChildInfos[index].resourceType = resource->GetResourceType();
					ChildInfos[index].guid = resource->GetId();
					ChildInfos[index].path = resource->GetPath().c_str();
					auto folder = dynamic_cast<FolderResource*>(resource);
					if (folder != nullptr)
					{
						ChildInfos[index].childrenCount = folder->GetChildResources().size();
						ChildInfos[index].hasChildren = ChildInfos[index].childrenCount > 0;
					}
					
					index++;
				}
			}
			return true;
		}
	}
	return false;
}


bool CreateResource(ResourceType type, ResourceID parent, char* path, __out ResourceInfo& resourceInfo)
{
	auto resourceSystem = EngineManager::Get()->GetResourceSystem();
	string pathStr = string(path);
	auto resource=resourceSystem->CreateResource(type, parent, pathStr);
	if (resource != nullptr)
	{
		resourceInfo.guid = resource->GetId();
		resourceInfo.path = path;
		resourceInfo.childrenCount = resource->GetChildResources().size();
		resourceInfo.hasChildren = resourceInfo.childrenCount > 0;
		resourceInfo.resourceType = resource->GetResourceType();
		return true;
	}
	return false;
}
bool DeleteResource(ResourceID id, ResourceID parentid)
{
	auto resourceSystem = EngineManager::Get()->GetResourceSystem();
	resourceSystem->DeleteResource(id, parentid);
	return true;
}

int GetInMemeryResourceCount()
{
	auto resourceManager = EngineManager::Get()->GetResourceSystem()->GetResourceManager();
	return resourceManager->GetInMemeryResourceCount();
}
bool GetAllInMemeryResourceInfos(ResourceInfo* infos,__inout int& size)
{
	
	auto resourceManager = EngineManager::Get()->GetResourceSystem()->GetResourceManager();
	if (resourceManager->GetInMemeryResourceCount() == size)
	{
		int index = 0;
		auto resourceIds = resourceManager->GetInMemeryResourceIDs();
		for (auto& id : resourceIds)
		{
			auto resource=resourceManager->GetResource(id);
			if (resource != nullptr)
			{
				infos[index].guid = resource->GetId();
				infos[index].resourceType = resource->GetResourceType();
				infos[index].path = resource->GetPath().c_str();
				index++;
			}
		}
		size = index;
		return true;
	}
	size = 0;
	return false;
}

