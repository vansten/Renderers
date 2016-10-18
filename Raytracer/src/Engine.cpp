#include "../include/Engine.h"
#include "../include/Scene.h"
#include "Console.h"

Engine* Engine::_instance = 0;

Engine::Engine() : _scene(0)
{
	_instance = this;
}


Engine::~Engine()
{
}

bool Engine::Init(HINSTANCE hInstance, LPSTR commandLine, int nCmdShow)
{
	Console::Open();

	WNDCLASS windowClass = {};
	windowClass.hInstance = hInstance;
	windowClass.lpfnWndProc = WindowsProcedure;
	windowClass.lpszClassName = "vanstenRaytracerWindowClass";
	windowClass.style = CS_VREDRAW | CS_HREDRAW;

	if (!RegisterClass(&windowClass))
	{
		Console::WriteLine("Can't register window class\n");
		return false;
	}

	_windowHandle = CreateWindow(windowClass.lpszClassName, "vansten Raytracer", WS_OVERLAPPEDWINDOW, 100, 100, 1280, 720, 0, 0, hInstance, 0);
	if (!_windowHandle)
	{
		Console::WriteLine("Couldn't create window");
		return false;
	}

	ShowWindow(_windowHandle, nCmdShow);

	if (!SetupScene())
	{
		Console::WriteLine("Couldn't setup scene");
		return false;
	}

	return true;
}

void Engine::Shutdown()
{
	if (_scene)
	{
		_scene->Shutdown();
		delete _scene;
		_scene = 0;
	}

	CloseWindow(_windowHandle);
	DestroyWindow(_windowHandle);

	system("pause");
	Console::Close();
}

void Engine::Run()
{
	MSG msg;
	_bRunning = true;
	while (_bRunning)
	{
		while (PeekMessage(&msg, _windowHandle, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		_scene->Update();
		_scene->RenderUnlit();
	}

	CloseWindow(_windowHandle);
}

void Engine::Exit()
{
	_bRunning = false;
}

Engine* Engine::GetInstance()
{
	return _instance;
}

bool Engine::SetupScene()
{
	_scene = new Scene();
	if (!_scene)
	{
		return false;
	}
	_scene->Init();
	
	return true;
}

LRESULT CALLBACK WindowsProcedure(HWND windowHandle, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_QUIT:
	case WM_CLOSE:
	case WM_DESTROY:
		Engine::GetInstance()->Exit();
		break;
	case WM_KEYUP:
		if (wParam == VK_ESCAPE)
		{
			Engine::GetInstance()->Exit();
		}
		break;
	}
	return DefWindowProc(windowHandle, msg, wParam, lParam);
}
