#pragma once

typedef void (*ErrorCallBack)(char* ErrorMessage);
typedef void (*GraphicsErrorCallBack)(int hr, const char* errorMessage,const char* filename, const char* functionName,int LineNuamber);

//GraphicsErrorCallBack LogGraphicsErrorCallBack;
//
//void SetGraphicsErrorCallBack(GraphicsErrorCallBack callBack)
//{
//	LogGraphicsErrorCallBack = callBack;
//}