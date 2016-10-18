#pragma once

#include "DeviceContext.h"
#include "Input.h"
#include "Scene.h"

class System
{
protected:
	DeviceContext* _deviceContext;
	Input* _input;
	Scene* _scene;

public:
	static bool bRunning;

public:
	System();
	~System();

	bool Init(HINSTANCE hInstance, LPSTR commandLine, int nCmdShow);
	void Shutdown();
	void Run();

	void MakePrintScreen();
};

