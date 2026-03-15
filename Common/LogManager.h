#pragma once

#include"LogCallBack.h"
#include<memory>
//#include"spdlog/logger.h"
enum LogLevel :int
{
	Info,
	Debug,
	Warning,
	Error,
};

static class LogManager
{
private:
	static LogMessageCallback _LogMessageCallback;
	//static std::shared_ptr<spdlog::logger> logger;
public:
	 LogManager();

	 static void SetLogMessageCallback(LogMessageCallback callback);

	 static void Init();

	 static void Log(LogLevel level, const char* message);

	/* static void Log(LogLevel level,char* message, char* fileName, char functionName, int line);

	 static void Error(char* message);
	 static void Warning(char* message);
	 static void Debug(char* message);*/
	 //static void 
	 ~LogManager();
};