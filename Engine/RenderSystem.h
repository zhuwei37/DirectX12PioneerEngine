#pragma once
#include"interface.h"
#include"Renderer.h"
#include"RenderHost.h"
#include"Scene.h"
#include<map>
#include <ShaderResourceView.h>
#include"SkyBox.h"

using namespace std;
class RenderHostGroup;
using RenderHostGroupPtr = std::shared_ptr<RenderHostGroup>;
class RenderSystem
{
public:
	//‰÷»æ¥∞ø⁄µƒ¿‡–Õ
	enum RenderHostType:int
	{
		MainRendering,
		MaterialRendering,
		AnimationRendering
	};


	
public:
	RenderSystem(GraphicsDevice*);


	RenderHostId CreateRenderHost(CreateRenderHostParameter parameter);
	void DestoryRenderHost(RenderHostId,  RenderHostType type = RenderHostType::MainRendering);
	/// <summary>
	/// º§ªÓ‰÷»æ∆˜
	/// </summary>
	/// <param name="renderer"></param>
	void ActivateRenderer(Renderer* renderer,RenderHostType type=RenderHostType::MainRendering);
	/// <summary>
	/// πÿ±’‰÷»æ∆˜ 
	/// </summary>
	/// <param name="renderer"></param>
	void CloseRenderer(Renderer* renderer,  RenderHostType type = RenderHostType::MainRendering);
	void PreRender();
	void OnRender();

	RenderHost* GetRenderHost(RenderHostId id);
	

//	std::unique_ptr<ShaderResourceView> mSrv;
private:
	RenderHostType mCurrentRenderHostType = RenderHostType::MainRendering;
private:
	RenderHostId GetNextRenderHostId();
	RenderHostId RenderHostIdCounter=1;
	std::map<RenderHostId, shared_ptr<RenderHost>> mRenderHostMap;
private:
	

	vector<RenderHost*> mActiveRenderHosts;

	std::vector<RenderHostGroupPtr> mRenderHostGroups;


private:
	GraphicsDevice* mGDevice;
};