#include"SceneNode.h"

#include"ActorManager.h"
SceneNode::SceneNode(const char* name,SceneNodeID selfID,
	SceneNodeID parentId):
	Name(name),
	mParentID(parentId),
	mSelfID(selfID)
{
}
SceneNode::SceneNode(const char* name,SceneNodeID selfID):
	Name(name),
	mSelfID(selfID),
	mParentID(0)
{
	
}
 const string& SceneNode::GetName()
{
	return Name;
}

 const SceneNodeID SceneNode::GetID()
 {
	 return mSelfID;
 }
 const SceneNodeID SceneNode::GetParentID()
 {
	 return mParentID;
 }
 void SceneNode::SetName(const char* newName)
 {
	 Name.assign(newName);
 }
 void SceneNode::AddChildren(shared_ptr<ISceneNode> children)
 {
	 auto id=children->GetID();
	 auto it=SceneNodeMap.find(id);
	 if (it == SceneNodeMap.end())
	 {
		 SceneNodeMap[id] = children;
	 }
 }
 void SceneNode::RemoveChildren(SceneNodeID ID)
 {
	 auto it = SceneNodeMap.find(ID);
	 if (it != SceneNodeMap.end())
	 {
		 SceneNodeMap.erase(it);
	 }
 }
 SceneNode::~SceneNode()
 {
	  ActorManager::Get()->RemoveActor( this->mActorId);
 }