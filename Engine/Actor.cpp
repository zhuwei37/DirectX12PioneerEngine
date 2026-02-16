#include"Actor.h"
#include"ActorManager.h"
void Actor::Update()
{

}
Actor::Actor(ActorId Id):
	mId(Id)
{

}
void Actor::Destory()
{
	for (auto component : mComponentMap)
	{
		component.second->Destory();
	}
	mComponentMap.clear();
}
void Actor::AddComponent(std::shared_ptr<IComponent> component)
{
	auto id = component->GetId();
	mComponentMap[id] = component;
}
void Actor::RemoveComponent(ComponentId id)
{
	auto it = mComponentMap.find(id);
	if (it != mComponentMap.end())
	{
		mComponentMap.erase(it);
	}
}
Layer Actor::GetLayer()
{
	return this->mLayer;
}
void Actor::SetLayer(Layer layer)
{
	this->mLayer = layer;
}
void Actor::RemoveComponent(const char* name)
{
	auto id=IComponent::GetIdFromName(name);
	RemoveComponent(id);
}
Actor::~Actor()
{
	int x=0;
	x += 1;
	//ActorManager::Get()->RemoveActor(mId);
}