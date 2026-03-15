#include"LogManager.h"
#include"ExportLog.h"

void LogMessage(LogLevel level, char* message)
{
	LogManager::Log(level, message);
}

void SetLogMessageCallback(LogMessageCallback callback)
{
	LogManager::SetLogMessageCallback(callback);
}