#include"EngineManager.h"
#include"RenderSystem.h"
void EngineManager::Init()
{
	mActorManager = ActorManager::Get();
	mResourceSystem = std::make_shared<ResourceSystem>();

	mRenderSystem = std::make_shared<RenderSystem>(GetGraphicsDevice());
	mMainScene = std::make_shared<Scene>();
}

RenderSystem* EngineManager::GetRenderSystem()
{
	return mRenderSystem.get();
}
void EngineManager::Update()
{
	mMainScene->UpdateTransformMatrix();
	mRenderSystem->PreRender();
	mRenderSystem->OnRender();
}