#pragma once
#include"interface.h"
#include<map>
#include"Actor.h"
#include"ActorFactory.h"
#include"tamplates.h"
#include"ActorManager.h"
#include"ActorComponentFactory.h"
class ActorManager
{
public:
	static ActorManager* Get()
	{
		static ActorManager instance;
		return &instance;
	}
	ActorManager();
	ActorPtr GetActor(ActorId);

	ActorPtr CreateActor();
	bool AddComponentToActor(ActorId actorId, ComponentId componentId);
	bool AddComponentToActor(ActorId actorId, const char* componentName);
	void RemoveActor(ActorId actorId)
	{
		auto it = ActorMap.find(actorId);
		
		if (it != ActorMap.end())
		{
			it->second->Destory();
			ActorMap.erase(it);
		}
	}
	void RemoveComponentFromActor(ActorId actorId, ComponentId componentId)
	{
		auto actor = GetActor(actorId);
		if (actor != nullptr)
		{
			actor->RemoveComponent(componentId);
		}
	}
	
private:
	
	
    ActorId ActorIdCounter = 1;
	ActorComponentFactory mFactory;
	std::map<ActorId, ActorPtr> ActorMap;
protected:

};