#pragma once
#include"IResource.h"
#include<vector>
class FolderResource :public IResource
{
public:
	FolderResource(std::string& path, ResourceID id)
	{
		this->mId = id;
		this->mPath = path;
		this->mResourceType = ResourceType::FOLDER;
	}
	FolderResource() 
	{
		this->mResourceType = ResourceType::FOLDER;
	}
	inline bool HasChild()
	{
		return Children.size() > 0;
	}
	
	
	
private:

};