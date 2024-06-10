#pragma once


// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// 
// NOTE:
// 
// Les fonctions de log terminent prématurément le programme par un appel de
// la fonction exit() lorsque le niveau de log est ERROR_FATAL et ce même si
// le système de log lui-même n'est pas intialisé.
// 
// Cette technique est reprise du moteur de jeu UnrealEngine.
// 
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


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

// ---

void InitialiseLog(const char* filename);

void CloseLog();

// ---

#define LOG_ERRNO(...) LogErrno(__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__) 

void LogErrno(const char* file, int line, const char* func, LogLevel level, int error);

// ---

#define LOG(...) Log(__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__) 

void Log(const char* file, int line, const char* func, LogLevel level, const char* format, ...);
