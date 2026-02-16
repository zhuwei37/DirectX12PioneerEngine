#pragma once
#include"SceneNode.h"
class SceneNodeFactroy
{
public:
	shared_ptr<ISceneNode> CreateSceneNode(const char* name)
	{
		std::shared_ptr<ISceneNode> sceneNode = std::make_shared<SceneNode>(name, GetNewSceneNodeID());
		return sceneNode;
	}
	shared_ptr<ISceneNode> CreateSceneNode(const char* name,SceneNodeID parentId)
	{
		std::shared_ptr<ISceneNode> sceneNode = std::make_shared<SceneNode>(name, GetNewSceneNodeID(),parentId);
		return sceneNode;
	}
private:
	SceneNodeID GetNewSceneNodeID()
	{
		return SceneNodeCounter++;
	}
private:
	SceneNodeID SceneNodeCounter = 1;
};