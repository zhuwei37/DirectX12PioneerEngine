#pragma once
#include"Renderer.h"
#include"RenderSystem.h"
#include"RenderHost.h"
#include<map>
#include<vector>
#include"interface.h"
using RendererList = std::vector<Renderer*>;
class RenderHostGroup
{
	friend class RenderSystem;
public :
	
public:
	RenderHostGroup(RenderSystem::RenderHostType type);

	void AddRenderer(Renderer*);
	void RemoveRenderer(Renderer*);
	void AddRenderHost(RenderHost*);
	void RemoveRenderHost(RenderHost*);
	void PreRender();
	std::vector<ComPtr< ID3D12GraphicsCommandList>> OnRender();
	RenderSystem::RenderHostType GetRenderHostType();
private:
	SkyBox mSkyBox;


	RenderSystem::RenderHostType mRenderHostGroupType;

	std::map<Layer, RendererList> mRendererMap;
	std::vector<RenderHost*> mActiveRenderHosts;
	std::vector<RenderHost*> mRenderHostList;
};