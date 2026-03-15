//#include"spdlog/spdlog.h"
//#include"spdlog/sinks/basic_file_sink.h"
#include"LogManager.h"
#include<chrono>
LogMessageCallback LogManager::_LogMessageCallback = nullptr;
LogManager::LogManager()
{
	/*spdlog::flush_every(std::chrono::seconds(5));
	logger= spdlog::basic_logger_mt("file_logger", "logs/log.log");
	logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] [thread %t] %v");
	logger->set_level(spdlog::level::info);*/
	
}
void LogManager::Init()
{

}
void LogManager::SetLogMessageCallback(LogMessageCallback callback)
{
	_LogMessageCallback = callback;
}
void LogManager::Log(LogLevel level,const char* message)
{
	if (_LogMessageCallback != nullptr)
	{
		_LogMessageCallback(level, message);	
	}
	switch (level)
	{
	case LogLevel::Info:
	{
		//logger->info(message);
		break;
	}
	case LogLevel::Debug:
	{
		//logger->debug(message);
		break;
	}
	case LogLevel::Warning:
	{
		//logger->warn(message);
		break;
	}
	case LogLevel::Error:
	{
		//logger->error(message);
		break;
	}
	default:
		break;
	}
}

LogManager::~LogManager()
{

}