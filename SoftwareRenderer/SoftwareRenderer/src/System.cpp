#include "../include/System.h"

#include <conio.h>

#include "TGASerializer.h"

//#include "Profiler.h"

#include "Matrix.h"

#include "Timer.h"

#include <sstream>
#include <iomanip>

#define USE_CONSOLE 0

#if USE_CONSOLE
#include "Console.h"
#endif

template <typename T>
std::string to_string_with_precision(const T a_value, const int n = 6)
{
	std::ostringstream out;
	out << std::setprecision(n) << a_value;
	return out.str();
}

bool System::bRunning = false;

System::System() : _deviceContext(0)
{
}

System::~System()
{
}

bool System::Init(HINSTANCE hInstance, LPSTR commandLine, int nCmdShow)
{
#if USE_CONSOLE
	if(!Console::Open())
	{
		return false;
	}
#endif

	_deviceContext = new DeviceContext();
	if(!_deviceContext)
	{
		return false;
	}

	if(!_deviceContext->Init(hInstance, commandLine, nCmdShow))
	{
		return false;
	}

	_input = new Input();
	_input->Init();

	_scene = new Scene();
	if(!_scene || (!_scene->Init()))
	{
		return false;
	}
	
	return true;
}

void System::Shutdown()
{
	if(_deviceContext)
	{
		_deviceContext->Shutdown();
		delete _deviceContext;
		_deviceContext = 0;
	}

	if(_input)
	{
		delete _input;
		_input = 0;
	}

	if(_scene)
	{
		_scene->Shutdown();
		delete _scene;
		_scene = 0;
	}

#if USE_CONSOLE
	printf("Press key to shutdown system...");
	_getch();
	Console::Close();
#endif
}

void System::Run()
{
	bRunning = true;
	float dt = 1.0f / 60.0f;
	LARGE_INTEGER start;
	LARGE_INTEGER current;
	LARGE_INTEGER elapsed;
	Timer* timer = new Timer();
	float time = 0.0f;
	float fps;
	std::string fpsString;
	int frames = 0;
	while(bRunning)
	{
		timer->Start();
		_deviceContext->Clear(0xFF, 0xFF, 0xFF, 0xFF);
		_input->Update();
		_deviceContext->Update();
		
		_scene->Update(dt);
		_scene->Draw(_deviceContext);

		_deviceContext->SwapBuffers();
		dt = timer->GetElapsedTime();
		time += dt;
		++frames;
		if(time > 0.5f)
		{
			fps = frames / time;
			fpsString = "FPS: " + to_string_with_precision(fps, 4);
			frames = 0;
			time = 0.0f;
		}
		TextOut(_deviceContext->_deviceContextHandle, 0, 0, fpsString.c_str(), fpsString.size());
	}
	delete timer;
	timer = 0;
}

void System::MakePrintScreen()
{
	if(_deviceContext)
	{
		_deviceContext->PrintScreen();
	}
}