#pragma once
#include<string>
#include<memory>
using namespace std;
typedef unsigned int SceneNodeID;
class ISceneNode
{
public:
	//virtual void PreRender() = 0;
	//virtual void Update() = 0;
	//virtual bool IsVisible()const = 0;
//	virtual shared_ptr<ISceneNode> GetParent()=0;
	virtual void AddChildren(shared_ptr<ISceneNode> children)=0;
	virtual void RemoveChildren(SceneNodeID actorId) = 0;
	virtual const string& GetName()  = 0;
	virtual void SetName(const char* newName)=0;
	virtual const SceneNodeID GetID()  = 0;
	virtual const SceneNodeID GetParentID()  = 0;
	//virtual  SceneId GetSceneId() = 0;
	//virtual int GetChildrenCount() = 0;
	virtual ~ISceneNode() {};
};