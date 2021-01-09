#include "Application.h"
#include "System/Clock.h"

b8 s_Running = TRUE;

int main(int argc, char** argv)
{
	ApplicationInitialize();

	ClockInitialize();
	f64 delta, time, lastTime = ClockGetTime();
	while (s_Running)
	{
		time = ClockGetTime();
		delta = time - lastTime;
		lastTime = time;

		ApplicationUpdate(delta);
	}

	ApplicationShutdown();
}
