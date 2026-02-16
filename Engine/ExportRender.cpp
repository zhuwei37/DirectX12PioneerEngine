#include"ExportRender.h"
#include"RenderSystem.h"
#include"RenderHost.h"
#include"EngineManager.h"
unsigned int CreateRenderHost( int hwnd, int width, int height)
{
	auto engineManager= EngineManager::Get();
	auto renderSystem= engineManager->GetRenderSystem();
	CreateRenderHostParameter parameter;
	parameter.Height = height;
	parameter.Width = width;
	parameter.WindowHandle = hwnd;
	return renderSystem->CreateRenderHost(parameter);
}
void Resize_RenderHost(unsigned int renderHostId, int width, int height)
{
	auto renderSystem = EngineManager::Get()->GetRenderSystem();
	auto renderHost = renderSystem->GetRenderHost(renderHostId);
	if (renderHost != nullptr)
	{
		renderHost->Resize(width, height);
	}
}
void Destory_RenderHost(unsigned int renderHostId)
{
	auto renderSystem = EngineManager::Get()->GetRenderSystem();
	renderSystem->DestoryRenderHost(renderHostId);
}
bool GetRenderHostVisibility(unsigned int renderHostId)
{
	auto renderSystem = EngineManager::Get()->GetRenderSystem();
	auto renderHost = renderSystem->GetRenderHost(renderHostId);
	if (renderHost != nullptr)
	{
		return renderHost->IsVisible();
	}
	return false;
}
void SetRenderHostVisibility(unsigned int renderHostId, bool vis)
{
	auto renderSystem = EngineManager::Get()->GetRenderSystem();
	auto renderHost = renderSystem->GetRenderHost(renderHostId);
	if (renderHost != nullptr)
	{
		renderHost->SetVisible(vis);
	}
}