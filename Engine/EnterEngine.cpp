#include"EnterEngine.h"
#include "CrateApp.h"
#include "SharedTexture.h"
#include "iostream"
#include <string>
#include <locale>
#include <codecvt>
#include"EngineManager.h"
#include"RenderManager.h"



void InitEngine()
{
	EngineManager::Get()->Init();
}

void Update()
{
	auto engineManager = EngineManager::Get();
	engineManager->Update();
}


void InitD3D(HWND hWnd, unsigned int width, unsigned int height)
{
	/*try
	{
		auto manager = RenderManager::Get();
		manager->Init(hWnd,width,height);
		
	}
	catch (GraphicsException ex)
	{
		if (graphicsErrorCallBack != nullptr)
		{
			graphicsErrorCallBack(ex.ErrorCode,ex.Message.c_str(), ex.Filename.c_str(),ex.FunctionName.c_str(), ex.LineNumber);
		}
	}*/
	
}
void SetGraphicsErrorCallBack(GraphicsErrorCallBack callback)
{
	graphicsErrorCallBack = callback;

}
void Resize(int width, int height)
{
	/*auto manager = RenderManager::Get();
	manager->OnResize(width, height);*/
}


unsigned int GetSharedTextureHandle()
{
	//try
	//{
	//	auto manager = RenderManager::Get();
	//	auto handle=manager->GetSharedTextureHandle();
	//	//MessageBox(nullptr, std::to_string((long)handle).c_str(), "Testr", MB_OK);
	//	return (unsigned int)handle;
	//}
	//catch (DxException ex)
	//{
	///*	auto message = ex.ToString();
	//	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	//	auto str = converter.to_bytes(message);
	//	MessageBox(nullptr, str.c_str(), "Testr", MB_OK);*/
	//}
	return 0;
}
void Run()
{
	/*try
	{
		auto manager = RenderManager::Get();
		manager->Draw();

	}
	catch (DxException ex)
	{
		auto message = ex.ToString();
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		auto str = converter.to_bytes(message);
		MessageBox(nullptr, str.c_str(), "Testr", MB_OK);
	}*/
}