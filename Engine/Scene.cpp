#include"Scene.h"
#include<queue>
#include"Actor.h"
#include"Transform.h"
SceneNodeID Scene::AddSceneNode(const char* sceneNodeName,unsigned int parentId)
{
	auto node = mFacotry.CreateSceneNode(sceneNodeName,parentId);

	return AddSceneNode(node, parentId);
}
SceneNodeID Scene::AddSceneNode(shared_ptr<ISceneNode> sceneNode, SceneNodeID parentId)
{
	
	if (parentId == 0)
	{
		mRoot->AddChildren(sceneNode);
		mSceneNodeMap[sceneNode->GetID()] = sceneNode;
		return sceneNode->GetID();
	}
	auto it = mSceneNodeMap.find(parentId);
	if (it != mSceneNodeMap.end())
	{
		auto parent = it->second;
		parent->AddChildren(sceneNode);
		
		mSceneNodeMap[sceneNode->GetID()] = sceneNode;

		return sceneNode->GetID();
	}
	return 0;
}
void Scene::SceneNodeBindActor(SceneNodeID sceneNodeId, Actor* actor)
{
	auto it = mSceneNodeMap.find(sceneNodeId);
	if (it != mSceneNodeMap.end())
	{
		auto sceneNode = dynamic_cast<SceneNode*>(it->second.get());
		if (sceneNode != nullptr)
		{
			sceneNode->mActorId = actor->GetId();
			auto transformWeakPtr = actor->GetComponent<Transform>("Transform");
			auto transformPtr= transformWeakPtr.lock().get();
			sceneNode->mTransform = transformPtr;
			if (sceneNode->mParentID != 0)
			{
				auto root = dynamic_cast<SceneNode*>(
					GetSceneNodeByID(sceneNode->mParentID));
				if (root != nullptr)
				{
					auto parentTransform = root->mTransform;
					parentTransform->AddTransformChildren(transformPtr);
					transformPtr->SetParent(parentTransform);
				}
			}
		}
	}
}
void Scene::UpdateTransformMatrix()
{
	for (auto& it : mRoot->SceneNodeMap)
	{
		dynamic_cast<SceneNode*>(it.second.get())->mTransform->UpdateMatrix();
	}
}
void Scene::RemoveSceneNode(SceneNodeID id)
{
	auto it = mSceneNodeMap.find(id);
	if (it != mSceneNodeMap.end())
	{
		auto parent = (SceneNode*)mSceneNodeMap[it->second->GetParentID()].get();
		parent->RemoveChildren(id);
		mSceneNodeMap.erase(it);
	}
}

ISceneNode* Scene::GetRootSceneNode()const
{
	return mRoot.get();
}

ISceneNode* Scene::GetSceneNodeByID(SceneNodeID id)const
{
	auto it = mSceneNodeMap.find(id);
	if (it != mSceneNodeMap.end())
	{
		return it->second.get();
	}
	return nullptr;
}

bool   Scene::GetAllSceneNodeInfos(SceneNodeInfo* data, int len)
{
	if (len != GetAllSceneNodeCount())
	{
		return false;
	}
	queue<SceneNode*> nodes;
	auto root=mRoot.get();
	nodes.push(root);
	data[0].ChildCount = root->SceneNodeMap.size();
	int index = 1;
	while (!nodes.empty())
	{
		SceneNode* parentNode = nodes.front();
		nodes.pop();
		int count = 0;
		for (auto node : parentNode->SceneNodeMap)
		{
			SceneNode* sceneNode = dynamic_cast<SceneNode*>(node.second.get());
			if (sceneNode != nullptr)
			{
				nodes.push(sceneNode);
				unsigned int id = sceneNode->GetID();
				unsigned int parentid = sceneNode->GetParentID();
				const string& name = sceneNode->GetName();
				memset(data[index].Name, 0, SceneNodeNameMaxLength);
				memcpy(data[index].Name, name.c_str(), name.size());
				data[index].ParentID = parentid;
				data[index].SceneNodeID = id;
				data[index].ActorId = sceneNode->GetActorId();
				data[index].ChildCount = sceneNode->SceneNodeMap.size();
				index++;
			}
		}
	}
	return true;
}