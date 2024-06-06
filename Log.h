#pragma once

enum LogLevel
{
	ACTION,
	ARGS,
	INFO,
	WARNING,
	ERROR_SEVERE,
	ERROR_FATAL
};

const char* LogLevelString(LogLevel level);

void InitialiseLog(const char* filename);
void Log(LogLevel level, const char* format, ...);
void CloseLog();
