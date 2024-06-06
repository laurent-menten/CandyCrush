#pragma once

enum LogLevel
{
	ACTION,
	ARGS,
	RET,
	INFO,
	WARNING,
	ERROR_SEVERE,
	ERROR_FATAL
};

void InitialiseLog(const char* filename);
void LogErrno(const char* file, int line, const char* func, LogLevel level, int error);
void Log(const char* file, int line, const char* func, LogLevel level, const char* format, ...);
void CloseLog();

#define LOG_ERRNO(...) LogErrno(__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__) 
#define LOG(...) Log(__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__) 
