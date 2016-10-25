#pragma once

#include <Windows.h>

#include "Types.h"
#include "Block.h"

#include "../include/OrthoCamera.h"
#include "../include/PerspectiveCamera.h"

namespace raytracer
{
	class Image;
	class Scene;
	class Camera;

	enum class MenuCommands
	{
		RENDER_IMAGE
	};

	class Engine
	{
	protected:
		const int _windowWidth = 900;
		const int _windowHeight = 900;
		float _pixelWidth;
		float _pixelHeight;

	protected:
		std::vector<Block*> _blocks;
		Scene* _scene;
		Image* _renderedImage;
		Camera* _camera;
		HWND _windowHandle;
		int _threadCount;
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
		void RenderBlock(Block* b) const;
		void RenderBlocks(const int start, const int howMany) const;
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

		inline float GetPixelWidth() const
		{
			return _pixelWidth;
		}

		inline float GetPixelHeight() const
		{
			return _pixelHeight;
		}

	protected:
		bool SetupScene();
	};

	LRESULT CALLBACK WindowsProcedure(HWND windowHandle, UINT msg, WPARAM wParam, LPARAM lParam);
}