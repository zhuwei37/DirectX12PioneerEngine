#include"ActorManager.h"
ActorManager::ActorManager()
{
}
ActorPtr ActorManager::GetActor(ActorId id)
{
	auto it = ActorMap.find(id);
	if (it != ActorMap.end())
	{
		return it->second;
	}
	else
	{
		return nullptr;
	}
}
ActorPtr ActorManager::CreateActor()
{
	ActorId newId = ActorIdCounter++;
	ActorPtr actor = std::make_shared<Actor>(newId);
	ActorMap[newId] = actor;
	//±ØÐë´æÔÚ
	AddComponentToActor(newId, "Transform");
	return actor;
}
bool ActorManager::AddComponentToActor(ActorId actorId, const char* componentName)
{
	auto actor = GetActor(actorId);
	if (actor != nullptr)
	{
		auto component = mFactory.CreateComponent(componentName);
		if (component != nullptr)
		{
			component->SetOwner(actor);
			actor->AddComponent(component);
			component->Awake();
			return true;
		}
	}
	return false;
}
bool ActorManager::AddComponentToActor(ActorId actorId, ComponentId componentId)
{
	auto actor = GetActor(actorId);
	if (actor != nullptr&&!actor->HasComponent(componentId))
	{
		
		auto component = mFactory.CreateComponent(componentId);
		if (component != nullptr)
		{
			component->SetOwner(actor);
			actor->AddComponent(component);
			component->Awake();
			return true;
		}
	}
	return false;
}
