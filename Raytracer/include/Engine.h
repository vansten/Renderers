#pragma once

#include <Windows.h>

class Scene;

class Engine
{
protected:
	Scene* _scene;
	HWND _windowHandle;
	bool _bRunning;

	static Engine* _instance;
public:

	Engine();
	~Engine();

	bool Init(HINSTANCE hInstance, LPSTR commandLine, int nCmdShow);
	void Shutdown();
	void Run();
	void Exit();

	static Engine* GetInstance();

protected:
	bool SetupScene();
};

LRESULT CALLBACK WindowsProcedure(HWND windowHandle, UINT msg, WPARAM wParam, LPARAM lParam);