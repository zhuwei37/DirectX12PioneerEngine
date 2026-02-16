#pragma once
#include<vector>
#include<map>
#include"SceneNode.h"
#include"SceneNodeFactory.h"
#include"SceneNodeInfo.h"
#include"interface.h"
class Scene
{
	friend class SceneNode;
public:
	//渲染准备
	void PreRender()=delete;
	Scene()
	{
		string str = "__Root__SceneNode__";
		mRoot = make_shared<SceneNode>(str.c_str(), 0);
		mSceneNodeMap[0] = mRoot;
		SceneID = 1;
	}
    
	// 节点操作 如 节点移动 删除节点
	//节点管理
	SceneNodeID AddSceneNode( const char* sceneNodeName, SceneNodeID parentId);
	SceneNodeID AddSceneNode(shared_ptr<ISceneNode> sceneNode, SceneNodeID parentId);

	void SceneNodeBindActor(SceneNodeID sceneNodeId, Actor* actor);

	void RemoveSceneNode(SceneNodeID sceneNodeId);
	
	ISceneNode* GetRootSceneNode()const;
	ISceneNode* GetSceneNodeByID(unsigned int sceneNodeId)const;
	bool GetAllSceneNodeInfos(SceneNodeInfo* data, int len);
	int GetAllSceneNodeCount()
	{
		return mSceneNodeMap.size();
	}
	void UpdateTransformMatrix();
	SceneId SceneID;
	//变换矩阵入栈操作
	//变换矩阵出栈操作
private:
	SceneNodeFactroy mFacotry;
	// 根节点
	shared_ptr<SceneNode> mRoot;
	//变换矩阵的根
//	map<ActorId, Transform*> mRootTransformMap;
	//
	map<unsigned int, shared_ptr<ISceneNode>> mSceneNodeMap;
};