#ifndef ENGINE_ENTERENGINE_H
#define ENGINE_ENTERENGINE_H
#ifdef __cplusplus
#include"ExPort.h"
#include"iostream"
#include"LogCallBack.h"
#include"GraphicsException.h"
GraphicsErrorCallBack graphicsErrorCallBack;
extern "C"
{

#endif // !__cpluscplus



DLL_EXPORTS void SetGraphicsErrorCallBack(GraphicsErrorCallBack callback);




DLL_EXPORTS void InitD3D(HWND hWnd,unsigned int width,unsigned int height) ;


DLL_EXPORTS void Run();

DLL_EXPORTS void Resize(int width, int height);

DLL_EXPORTS unsigned int GetSharedTextureHandle();

DLL_EXPORTS void Update();

DLL_EXPORTS void InitEngine();

#ifdef  __cplusplus
}
#endif // !__cplusplus





#endif
