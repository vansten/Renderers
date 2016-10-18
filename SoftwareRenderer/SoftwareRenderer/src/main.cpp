//#include <crtdbg.h>

#include "../include/System.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR commandLine, int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG); 

	{
		System* s = new System();
		if(s && s->Init(hInstance, commandLine, nCmdShow))
		{
			s->Run();
			s->Shutdown();
		}

		if(s)
		{
			delete s;
			s = 0;
		}
	}

	_CrtDumpMemoryLeaks();

	return 0;
}