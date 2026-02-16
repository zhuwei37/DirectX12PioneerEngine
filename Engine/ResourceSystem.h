#pragma once
#include"IResource.h"
#include"ResourceManager.h"
#include"FolderResource.h"
#include"boost/filesystem.hpp"
#include<map>
#include<objbase.h>
#include"IResourceProcessor.h"
#include"interface_resource.h"
class ResourceSystem
{
public:
	ResourceSystem();
	~ResourceSystem();
	ResourceID NewID();
	ResourceType CheckResourceType(boost::filesystem::path& path);
	ResourceManager* GetResourceManager();
//	static boost::filesystem::path GetDescriptionPath(boost::filesystem::path& path);
	//IResource CreateResource();

	IResource* CreateResource(ResourceType type, ResourceID parentId, std::string& pathStr);
	
	bool DeleteResource(ResourceID& id,ResourceID& parentId);
private:
	void DeleteResourceChildren(IResource*);
	void init();
	void LoadAssetRoot();
	void registerProcessor();
	void registerInMemeryResource();
	void LoadAsset(boost::filesystem::path& path, std::shared_ptr<FolderResource>  folder);
	std::shared_ptr<IResource> LoadResource(boost::filesystem::path& path);
	
	ResourceManager* mResourceManager;
	std::string metaExtension=".meta";
	std::shared_ptr<FolderResource> mRootFolder;
	boost::filesystem::path mRootPath;
private:
	std::map<ResourceType, IResourceProcessor*> ResourceProcessorMap;
	IResourceProcessor* ActivityProcessor;
	//FolderProcessor mFolderProcessor;
	
};