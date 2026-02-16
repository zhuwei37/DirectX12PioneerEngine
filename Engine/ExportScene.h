#pragma once
#include "ExPort.h"
#include"SceneNodeInfo.h"
#include"interface.h"
#include"SceneNode.h"
extern "C"
{
	DLL_EXPORTS int GetSceneNodeCount();
	DLL_EXPORTS bool GetAllSceneNodeInfos(SceneNodeInfo* data, int len);

	DLL_EXPORTS bool GetSceneNodeInfosByID(unsigned int id, SceneNodeInfo* data, int len);
	DLL_EXPORTS SceneNodeInfo CreateSceneNode(const char* name ,unsigned int parentid);
	DLL_EXPORTS void DeleteSceneNode(unsigned int sceneNodeID);


	DLL_EXPORTS void SetSceneNodeName(unsigned int id, char* newName);


	//DLL_EXPORTS bool AddComponent(SceneNodeID sceneNodeId, ComponentId compoentId);
	//DLL_EXPORTS bool RemoveComponent(SceneNodeID sceneNodeId, ComponentId compoentId);


	DLL_EXPORTS ComponentId StringToComponentId(const char* componentName);
	
	DLL_EXPORTS void LoadScene();


	DLL_EXPORTS void SetAll();



}
