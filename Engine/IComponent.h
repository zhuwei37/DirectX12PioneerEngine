#pragma once
#include"interface.h"
#include"String.h"
class IComponent
{
	friend class ActorFactory;
	friend class Actor;
	friend class ActorManager;
	friend class ComponentFactory;
public:
	virtual void Awake() = 0;
	virtual void Update() = 0;
	virtual ComponentId GetId()const { return GetIdFromName(GetName()); }
	virtual const char* GetName()const = 0;
	static ComponentId GetIdFromName(const char* componentStr)
	{
		void* rawId = HashedString::hash_name(componentStr);
		return reinterpret_cast<ComponentId>(rawId);
	}
	virtual ~IComponent()
	{
		m_pOwner.reset();
	}

	void SetIsEnable(bool isEnable)
	{
		mIsEnable = isEnable;
	}
	bool GetIsEnable()
	{
		return mIsEnable;
	}
	Layer GetLayer();
	
	//ISceneNode* GetSceneNode()
	//{
	//	return EngineManager::Get()->GetMainScene()->GetSceneNodeByID(m_pOwner->GetSceneNodeId());
	//}
	virtual void Destory()
	{
	}
private:
	
	void SetOwner(ActorPtr pOwner)
	{
		m_pOwner = pOwner;
	}
protected:
	bool mIsEnable=true;
	ActorPtr m_pOwner;
};