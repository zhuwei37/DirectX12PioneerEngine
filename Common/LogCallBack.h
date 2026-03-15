#pragma once

typedef void (*ErrorCallBack)(char* ErrorMessage);
typedef void (*LogMessageXCallback)(int level, char* message, char* fileName, char functionName,int line);
typedef void (*LogMessageCallback)(int level,const char* message);

typedef void (*GraphicsErrorCallBack)(int hr, const char* errorMessage,const char* filename, const char* functionName,int LineNuamber);

//GraphicsErrorCallBack LogGraphicsErrorCallBack;
//
//void SetGraphicsErrorCallBack(GraphicsErrorCallBack callBack)
//{
//	LogGraphicsErrorCallBack = callBack;
//}