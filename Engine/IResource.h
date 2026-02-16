#pragma once
#include<string>
#include<memory>
#include"ResourceType.h"
#include<vector>
#include"interface.h"

class IResource
{
	friend class ResourceManager;
	friend class ResourceSystem;
public:
	
	ResourceType GetResourceType()const
	{
		return mResourceType;
	}
	static ResourceID NewID()
	{
		ResourceID m_id;
		if (CoCreateGuid(&m_id)==S_OK)
		{
			return m_id;
		}
		return {};
	}
	inline void SetDirty(bool dirty) 
	{
		mDirty = dirty;
	}
	inline bool GetDirty()
	{
		return mDirty;
	}
	inline bool IsInMemory()
	{
		return mInMemory;
	}
	inline ResourceID GetId()
	{
		return mId;
	}
	inline void SetPath(std::string& path)
	{
		mPath = path;
	}
	inline std::string& GetPath()
	{
		return mPath;
	}
	inline std::vector<ResourceID>& GetChildResources()
	{
		return Children;
	}
	void AddChildResource(ResourceID guid)
	{
		Children.push_back(guid);
	}
	void RemoveChildResource(ResourceID& guid)
	{
		auto removeIt = std::remove_if(Children.begin(), Children.end(), [guid](ResourceID id)
			{
				return guid == id;
			});
		if (removeIt != Children.end())
		{
			Children.erase(removeIt);
		}
	}
	ResourceID GetParent()
	{
		return this->mParent;
	}
	void SetParent(ResourceID parentId)
	{
		this->mParent = parentId;
	}
	std::string& GetName()
	{
		return mName;
	}
	void SetName(std::string& newName) 
	{
		mName = newName;
	}
	virtual void AllReady(ResourceSystem* resourceSystem ) {}


	virtual std::string Serialize() { return ""; }


	virtual ~IResource()
	{
	}
protected:
	std::vector<ResourceID> Children;
	mutable ResourceType mResourceType;
	ResourceID mId;
	ResourceID mParent;
	
	bool mDirty;
	//
	bool mInMemory=false;
	std::string mPath;
	std::string mName;
};
