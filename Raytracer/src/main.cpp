#include "../include/Engine.h"
#include <stdio.h>
#include <crtdbg.h>

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR commandLine,
	int nCmdShow)
{
	{
		raytracer::Engine e;
		if (e.Init(hInstance, commandLine, nCmdShow))
		{
			e.Run();
		}
		e.Shutdown();
	}

#if _DEBUG
	_CrtDumpMemoryLeaks();
#endif

	return EXIT_SUCCESS;
}