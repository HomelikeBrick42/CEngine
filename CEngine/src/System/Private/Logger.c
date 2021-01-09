#include "System/Logger.h"
#include "System/SystemConsole.h"
#include "System/Defines.h"

#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static void AppendToLogFile(const char* message)
{
	// TODO: Actually write to file here.
}

void LogInitialize()
{
	// TODO: Create a new/wipe existing log file, then open it.
}

void LogShutdown()
{
	// TODO: If the file is open, close it.
}

void LogOutput(LogLevel level, const char* message, ...)
{
	char levelString[10];
	b8 isError = FALSE;
	switch (level)
	{
	case LOG_LEVEL_FATAL:
		strcpy(levelString, "[FATAL]: ");
		isError = TRUE;
		break;
	case LOG_LEVEL_ERROR:
		strcpy(levelString, "[ERROR]: ");
		isError = TRUE;
		break;
	case LOG_LEVEL_WARN:
		strcpy(levelString, "[WARN]:  ");
		break;
	case LOG_LEVEL_INFO:
		strcpy(levelString, "[INFO]:  ");
		break;
	case LOG_LEVEL_DEBUG:
		strcpy(levelString, "[DEBUG]: ");
		break;
	default:
	case LOG_LEVEL_TRACE:
		strcpy(levelString, "[TRACE]: ");
		break;
	}

	va_list args;
	va_start(args, message);
	char outMessage[32767];
	u64 outMessageLength;
#if PLATFORM_WINDOWS
	outMessageLength = _vsnprintf(outMessage, sizeof(outMessage) - 1, message, args);
#else
	outMessageLength = vsnprintf(outMessage, sizeof(outMessage), message, args);
#endif
	va_end(args);

	u64 totalLength = outMessageLength + 11;
	char* out = (char*)malloc(sizeof(char) * totalLength);
	sprintf(out, "%s%s", levelString, outMessage);
	out[totalLength - 2] = '\n';
	out[totalLength - 1] = '\0';

	if (isError)
		SystemConsolePrintError(out);
	else
		SystemConsolePrint(out);
	AppendToLogFile(out);

	free(out);
}
