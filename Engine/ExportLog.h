#pragma once
#include "ExPort.h"
#include"LogCallBack.h"
extern "C"
{

	DLL_EXPORTS void SetLogMessageCallback(LogMessageCallback callback);
	DLL_EXPORTS void LogMessage(LogLevel level,char* message);
	//DLL_EXPORTS void LogMessageX(int level, char* message, char* fileName, char functionName, int line);
}