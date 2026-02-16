#pragma once
#include"GraphicsDevice.h"
#include"Descriptor.h"
#include"GTexture.h"
/// <summary>
/// 管理g-buffer资源
/// </summary>
//class GBufferController
//{
//public:
//	enum  GBufferRTVOrder: int
//	{
//		WorldPosAndMatIndex,
//		GBufferLength,
//		Normal, 
//		Albedo,
//	};
//	GBufferController();
//	void OnResize();
//	void GetRtv(GBufferRTVOrder order);
//
//private:
//	void CreateRtv();
//	void CreateDsv();
//
//	Descriptor mRtvHandle;
//	Descriptor mDsvHandle;
//
//	
//	GraphicsDevice* mGDevice;
//};