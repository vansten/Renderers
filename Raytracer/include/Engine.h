#pragma once

#include <Windows.h>

#include "Types.h"
#include "Block.h"

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
	std::vector<Block*> _blocks;
	Scene* _scene;
	Image* _renderedImage;
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

	void Render();
	void SaveRenderedImage();

	void RenderScreenPixels(int left, int right, int bottom, int top, const Color24* pixels) const;
	void PresentBlockWork(const Block* block) const;

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