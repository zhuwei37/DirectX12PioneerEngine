#include"ExPortCamera.h"
#include"EngineManager.h"
#include"RenderSystem.h"
#include"RenderHost.h"
#include"CameraController.h" 
void ::Walk(unsigned int renderHostId, float d)
{
	auto engineManager = EngineManager::Get();
	auto renderHost = engineManager->GetRenderSystem()->GetRenderHost(renderHostId);
	if (renderHost != nullptr)
	{
		renderHost->GetCameraController()->Walk(d);
	}
}
void ::Strafe(unsigned int renderHostId, float d)
{
	auto engineManager = EngineManager::Get();
	auto renderHost = engineManager->GetRenderSystem()->GetRenderHost(renderHostId);
	if (renderHost != nullptr)
	{
		renderHost->GetCameraController()->Strafe(d);
	}
}

void ::Pitch(unsigned int renderHostId, float d)
{
	auto engineManager = EngineManager::Get();
	auto renderHost = engineManager->GetRenderSystem()->GetRenderHost(renderHostId);
	if (renderHost != nullptr)
	{
		renderHost->GetCameraController()->Pitch(d);
	}
}
void ::RotateY(unsigned int renderHostId, float d)
{
	auto engineManager = EngineManager::Get();
	auto renderHost = engineManager->GetRenderSystem()->GetRenderHost(renderHostId);
	if (renderHost != nullptr)
	{
		renderHost->GetCameraController()->RotateY(d);
	}
}