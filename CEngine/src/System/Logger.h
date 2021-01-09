#pragma once

#define LOG_WARN_ENABLED 1
#define LOG_INFO_ENABLED 1
#define LOG_DEBUG_ENABLED 1
#define LOG_TRACE_ENABLED 1

typedef enum LogLevel
{
	LOG_LEVEL_FATAL = 0,
	LOG_LEVEL_ERROR = 1,
	LOG_LEVEL_WARN  = 2,
	LOG_LEVEL_INFO  = 3,
	LOG_LEVEL_DEBUG = 4,
	LOG_LEVEL_TRACE = 5
} LogLevel;

void LogInitialize();
void LogShutdown();

void LogOutput(LogLevel level, const char* message, ...);

#if !defined(LOG_FATAL)
	#define LOG_FATAL(msg, ...) LogOutput(LOG_LEVEL_FATAL, msg, __VA_ARGS__)
#endif

#if !defined(LOG_ERROR)
	#define LOG_ERROR(msg, ...) LogOutput(LOG_LEVEL_ERROR, msg, __VA_ARGS__)
#endif

#if !defined(LOG_WARN)
	#if LOG_WARN_ENABLED
		#define LOG_WARN(msg, ...) LogOutput(LOG_LEVEL_WARN, msg, __VA_ARGS__)
	#else
		#define LOG_WARN(msg, ...) do {} while(0)
	#endif
#endif

#if !defined(LOG_INFO)
	#if LOG_INFO_ENABLED
		#define LOG_INFO(msg, ...) LogOutput(LOG_LEVEL_INFO, msg, __VA_ARGS__)
	#else
		#define LOG_INFO(msg, ...) do {} while(0)
	#endif
#endif

#if !defined(LOG_DEBUG)
	#if LOG_DEBUG_ENABLED
		#define LOG_DEBUG(msg, ...) LogOutput(LOG_LEVEL_DEBUG, msg, __VA_ARGS__)
	#else
		#define LOG_DEBUG(msg, ...) do {} while(0)
	#endif
#endif

#if !defined(LOG_TRACE)
	#if LOG_TRACE_ENABLED
		#define LOG_TRACE(msg, ...) LogOutput(LOG_LEVEL_TRACE, msg, __VA_ARGS__)
	#else
		#define LOG_TRACE(msg, ...) do {} while(0)
	#endif
#endif
