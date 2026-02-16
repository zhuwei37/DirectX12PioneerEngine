#pragma once
#include"interface.h"
#include<vector>
#include<map>
#include"IComponent.h"
#include"interface.h"
#include"SceneNode.h"

class Actor
{
public:
	Actor(ActorId);
	int GetComponentCount()
	{
		return mComponentMap.size();
	}
	std::vector<ComponentExportInfo> GetComponentInfos()
	{
		int index = 0;
		std::vector<ComponentExportInfo> infos(mComponentMap.size());
		for (auto& it : mComponentMap)
		{
			infos[index].ComponentId = it.first;
			infos[index].ComponentPtr = it.second.get();
			index++;
		}
		return infos;
	}
	IComponent* GetComponentPtr(ComponentId id)
	{
		auto it = mComponentMap.find(id);
		if (it != mComponentMap.end())
		{
			return it->second.get();
		}
		return nullptr;
	}
	void Update();
	void Destory();
	void RemoveComponent(ComponentId id);
	void RemoveComponent(const char* name);
	void AddComponent(std::shared_ptr<IComponent> component);
	bool HasComponent(ComponentId id)
	{
		auto it = mComponentMap.find(id);
		return it != mComponentMap.end();
	}
	void SetSceneNode(SceneNodeID id)
	{
		mSceneNodeId = id;
	}
	SceneNodeID GetSceneNodeId()const
	{
		return mSceneNodeId;
	}
	ActorId GetId()const { return mId; }
	 //GetSceneNode();
	template<class ComponentType>
	std::weak_ptr<ComponentType> GetComponent(const char* name)
	{
		auto id = IComponent::GetIdFromName(name);
		auto it = mComponentMap.find(id);
		if (it != mComponentMap.end())
		{
			std::shared_ptr<ComponentType> componentPtr ( std::static_pointer_cast<ComponentType>(it->second));
			std::weak_ptr<ComponentType> ptr(componentPtr);
			return ptr;
		}
		else
		{
			return std::weak_ptr<ComponentType>();
		}
	}
	template<class ComponentType>
	std::weak_ptr<ComponentType> GetComponent(ComponentId id)
	{
		auto it = mComponentMap.find(id);
		if (it != mComponentMap.end())
		{
			std::shared_ptr<ComponentType> componentPtr(std::static_pointer_cast<ComponentType>(it->second));
			std::weak_ptr<ComponentType> ptr(componentPtr);
			return ptr;
		}
		else
		{
			return std::weak_ptr<ComponentType>();
		}
	}
	Layer GetLayer();
	void SetLayer(Layer);
	~Actor();
	
private:
	ActorId mId;
	SceneNodeID mSceneNodeId;
	map<ComponentId, ActorComponentStrongPtr> mComponentMap;
	string mTag;
	Layer mLayer=Layer::Default;
};