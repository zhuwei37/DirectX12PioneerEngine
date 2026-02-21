#pragma once
#include"Scene.h"
//#include<assimp/Importer.hpp>
//#include <assimp\scene.h>
//#include <assimp\postprocess.h>
#include"Device.h"
#include"ActorManager.h"
#include"ResourceSystem.h"
#include<iostream>
#include"yaml-cpp/yaml.h"
class RenderSystem;
class EngineManager
{
public:
	static EngineManager* Get()
	{
		static EngineManager instance;
		return &instance;
	}
public:
	EngineManager()
	{
		
		//aiScene
		/*Assimp::Importer import;
		auto p=import.ReadFile("E:\\BaiduNetdiskDownload\\DxRun\\assimp-6.0.2\\test\\models\\FBX\\box.fbx", aiProcess_Triangulate |
			aiProcess_ConvertToLeftHanded);*/
		//aiScene
		
	
	
		
	}
	void Init();
	 void Update();
	Scene* GetMainScene()
	{
		return mMainScene.get();
	}
	RenderSystem* GetRenderSystem();
	ActorManager* GetActorManager()
	{
		return ActorManager::Get();
	}
	ResourceSystem* GetResourceSystem()
	{
		return mResourceSystem.get();
	}
	GraphicsDevice* GetGraphicsDevice()
	{
		return GraphicsDevice::Get();
	}
private:
	std::shared_ptr<Scene> mMainScene;
	std::shared_ptr<RenderSystem> mRenderSystem;
	ActorManager* mActorManager;
	std:: shared_ptr<ResourceSystem> mResourceSystem;
};