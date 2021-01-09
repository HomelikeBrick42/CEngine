#include "System/Clock.h"

#if PLATFORM_WINDOWS

#include <Windows.h>

static LARGE_INTEGER s_StartTime;
static f64 s_InverseFrequency;

void ClockInitialize()
{
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    s_InverseFrequency = 1.0 / (f64)frequency.QuadPart;
    QueryPerformanceCounter(&s_StartTime);
}

f64 ClockGetTime()
{
    static LARGE_INTEGER time;
    QueryPerformanceCounter(&time);
    return (f64)(time.QuadPart - s_StartTime.QuadPart) * s_InverseFrequency;
}

#else
	#error "Implement clock for this platform!"
#endif
