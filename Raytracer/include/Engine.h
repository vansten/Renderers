#pragma once

#include <Windows.h>

#include "Types.h"

class Image;
class Scene;

enum class MenuCommands
{
	RENDER_IMAGE
};

class Engine
{
protected:
	const int _windowWidth = 1280;
	const int _windowHeight = 720;

protected:
	Scene* _scene;
	HWND _windowHandle;
	bool _bRunning;

	Image* _renderedImage;

	static Engine* _instance;
public:

	Engine();
	~Engine();

	bool Init(HINSTANCE hInstance, LPSTR commandLine, int nCmdShow);
	void Shutdown();
	void Run();
	void Exit();

	void Render();
	void SaveRenderedImage();

	void RenderScreenPixels(int left, int right, int bottom, int top, const Color24* pixels)const;

	static Engine* GetInstance();

	inline int GetWidth() const
	{
		return _windowWidth;
	}

	inline int GetHeight() const
	{
		return _windowHeight;
	}

protected:
	bool SetupScene();
};

LRESULT CALLBACK WindowsProcedure(HWND windowHandle, UINT msg, WPARAM wParam, LPARAM lParam);