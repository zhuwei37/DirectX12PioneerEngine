#pragma once
#include"FolderResource.h"
#include"MeshResource.h"
#include"GUIDHelper.h"
#include<unordered_map>
class ResourceManager
{
	friend class ResourceSystem;
public:
	ResourceManager();
	IResource* GetResource(ResourceID& id);
	std::shared_ptr<IResource> GetResourceStrongPtr(ResourceID& id);
	std::weak_ptr<IResource> GetResourceWeakPtr(ResourceID& id);
	void AddResource(std::shared_ptr<IResource>);
	/// <summary>
	/// 删除资源
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	bool DeleteResouce(ResourceID&);
	FolderResource* GetRootFolderResource()const;


	int GetInMemeryResourceCount();
	std::vector<ResourceID> GetInMemeryResourceIDs();


private:
	void SetRootFolderResource(FolderResource*);
	mutable FolderResource* mRootFolderResource;

      std::unordered_map<ResourceID, std::shared_ptr<IResource>, GUIDHash, GUIDEqual> mResourceMap;
	  /// <summary>
	  /// 在内存中不存储在硬盘中的资源
	  /// </summary>
	  std::unordered_map<ResourceID, std::shared_ptr<IResource>, GUIDHash, GUIDEqual> mResourceInMemoryMap;
	
};