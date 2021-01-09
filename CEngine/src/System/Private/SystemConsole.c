#include "System/SystemConsole.h"
#include "System/Defines.h"

#if PLATFORM_WINDOWS

#include <Windows.h>

void SystemConsolePrint(const char* message)
{
	OutputDebugStringA(message);
	u64 length = strlen(message);
	LPDWORD number_written = NULL;
	WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), message, (DWORD)length, number_written, NULL);
}

void SystemConsolePrintError(const char* message)
{
	OutputDebugStringA(message);
	u64 length = strlen(message);
	LPDWORD number_written = NULL;
	WriteConsoleA(GetStdHandle(STD_ERROR_HANDLE), message, (DWORD)length, number_written, NULL);
}

#else
	#error "Implement system console for this platform!"
#endif
