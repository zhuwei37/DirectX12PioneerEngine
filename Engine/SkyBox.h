#pragma once
#include"GraphicsDevice.h"
#include"EngineManager.h"
#include"ActorManager.h"
class SkyBox
{
public :
	SkyBox();
	~SkyBox();
	Transform* Update();
	Mesh* GetMesh();
	void BeginRenderSkyBox();
	void EndRenderSkyBox();
private:
	void init();
	Mesh* skyBoxMesh;
    std::shared_ptr<Actor> mActor;
	Transform* transform;
};