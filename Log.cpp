
#include "Log.h"
#include "Affichage.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include <time.h>
#include <string.h>

static FILE* logFile;
static int messageId = 0;

void InitialiseLog(const char* filename)
{
	errno_t error = fopen_s(&logFile, filename, "a");
	if (error != 0)
	{
		char buffer[80];
		strerror_s(buffer, error);
		AfficheErreur("Impossible d'ouvrir le fichier de log '%s'\nErreur %d: %s\n", filename, error, buffer);

		exit(-1);
	}

	atexit(CloseLog);

	fprintf(logFile, " --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- \n");
}

static const char* LogLevelString(LogLevel level)
{
	switch (level)
	{
		case ACTION:		return "*";
		case ARGS:			return "  --->";
		case RET:			return "  =";

		case INFO:			return "I";
		case WARNING:		return "W";
		case ERROR_SEVERE:	return "E";
		case ERROR_FATAL:	return "F";
	}

	return "???";
}

void LogErrno(const char* file, int line, const char* func, LogLevel level, int error)
{
	char buffer[80];
	strerror_s(buffer, error);

	Log(file, line, func, level, "(%d)", error, buffer);
}

void Log(const char* file, int line, const char* func, LogLevel level, const char* format, ...)
{
	time_t t = time(NULL);
	struct tm tm;
	
	localtime_s(&tm, &t);

	char now[64];
	strftime(now, sizeof(now), "%Y-%m-%d %H:%M:%S", &tm );

	fprintf(logFile, "%d %10s: %s[%d] %s() %s ", messageId++, now, file, line, func, LogLevelString(level));

	va_list vargs;
	va_start(vargs, format);
	vfprintf(logFile, format, vargs);
	va_end(vargs);

	fprintf(logFile, "\n");
	fflush(logFile);

	if (level == ERROR_FATAL)
	{
		exit(-1);
	}
}

void CloseLog()
{
	AfficheMessage("Fermeture du fichier de log.");

	fclose(logFile);
}
