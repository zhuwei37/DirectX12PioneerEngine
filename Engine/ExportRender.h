#pragma once
#include"ExPort.h"
extern"C"
{
	DLL_EXPORTS unsigned int CreateRenderHost( int hwnd, int width, int height);
	DLL_EXPORTS void Resize_RenderHost(unsigned int, int width, int height);
	DLL_EXPORTS void Destory_RenderHost(unsigned int renderHostid);

	DLL_EXPORTS bool GetRenderHostVisibility(unsigned int renderHostId);
	DLL_EXPORTS void SetRenderHostVisibility(unsigned int renderHostId, bool vis);
}