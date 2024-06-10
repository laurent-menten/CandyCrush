
#include "Log.h"
#include "Affichage.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include <time.h>
#include <string.h>

static FILE* logFile = nullptr;
static int messageId = 0;

void InitialiseLog(const char* filename)
{
	errno_t error = fopen_s(&logFile, filename, "a");
	if (error != 0)
	{
		char buffer[80];
		strerror_s(buffer, error);
		AfficheErreur("Impossible d'ouvrir le fichier de log '%s'\n"
			"Erreur %d: %s\n", filename, error, buffer);

		exit(-1);
	}

	atexit(CloseLog);

	fprintf(logFile, " --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- \n");
}


void CloseLog()
{
	if (logFile == nullptr)
	{
		return;
	}

	// ---

	AfficheMessage("Fermeture du fichier de log.");

	fclose(logFile);
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
	if (logFile == nullptr)
	{
		return;
	}

	// ---

	char buffer[80];
	strerror_s(buffer, error);

	Log(file, line, func, level, "(errno = %d)", error, buffer);
}

void Log(const char* file, int line, const char* func, LogLevel level, const char* format, ...)
{
	if (logFile == nullptr && level != ERROR_FATAL)
	{
		return;
	}

	// ---

	time_t t = time(NULL);
	struct tm tm;

	localtime_s(&tm, &t);

	char now[64];
	strftime(now, sizeof(now), "%Y-%m-%d %H:%M:%S", &tm);

	char message[1024];
	va_list vargs;
	va_start(vargs, format);
	vsprintf_s(message, format, vargs);
	va_end(vargs);

	if (logFile != nullptr)
	{
		fprintf(logFile, "%d %10s: %s[%d] %s() %s: %s\n", messageId++, now, file, line, func, LogLevelString(level), message);

		fflush(logFile);
	}

	if (level == ERROR_FATAL)
	{
		AfficheErreur( message );
		exit(-1);
	}
}
