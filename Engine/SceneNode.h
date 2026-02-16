#pragma once
#include"ISceneNode.h"
#include<map>
#include"interface.h"
class Transform;
using namespace std;
class SceneNode :public ISceneNode
{
public:
	friend class Scene;
public :
	SceneNode(const char* sceneNodeName,SceneNodeID selfId);
	SceneNode(const char* sceneNodeName,SceneNodeID selfId,SceneNodeID parentId);
	virtual void AddChildren(shared_ptr<ISceneNode> children);
	virtual void RemoveChildren(SceneNodeID actorId);
	virtual  const string& GetName();
	virtual void SetName(const char* newName);
	virtual const SceneNodeID GetID();
	virtual const SceneNodeID GetParentID();

	ActorId GetActorId()const { return mActorId; }
	~SceneNode();
private:
	map<unsigned int,shared_ptr<ISceneNode>> SceneNodeMap;
	string Name;
	ActorId mActorId=0;
	SceneNodeID mSelfID;
	SceneNodeID mParentID;
	Transform* mTransform;
	
};