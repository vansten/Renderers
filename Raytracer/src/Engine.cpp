#include "../include/Engine.h"
#include "../include/Scene.h"
#include "../include/Console.h"
#include "../include/Image.h"
#include "../include/TGASerializer.h"
#include "../include/Timer.h"

Engine* Engine::_instance = 0;

Engine::Engine() : _scene(0), _renderedImage(0)
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

	_windowHandle = CreateWindow(windowClass.lpszClassName, "vansten Raytracer", WS_OVERLAPPEDWINDOW, 100, 100, _windowWidth, _windowHeight, 0, 0, hInstance, 0);
	if (!_windowHandle)
	{
		Console::WriteLine("Couldn't create window");
		return false;
	}

	HMENU mainMenu = CreateMenu();
	AppendMenu(mainMenu, MF_STRING, (UINT)MenuCommands::RENDER_IMAGE, "Render");

	SetMenu(_windowHandle, mainMenu);
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

	if(_renderedImage)
	{
		delete _renderedImage;
		_renderedImage = 0;
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
	}

	CloseWindow(_windowHandle);
}

void Engine::Exit()
{
	_bRunning = false;
}

void Engine::Render()
{
	Timer t;
	t.Start();
	if(_renderedImage)
	{
		delete _renderedImage;
		_renderedImage = 0;
	}

	_renderedImage = new Image(_windowWidth, _windowHeight, Color24::White * 0.3f);
	_scene->Render(_renderedImage);

	RenderScreenPixels(0, _windowWidth * 0.5f, 0, _windowHeight * 0.5f, _renderedImage->GetPixels());
	RenderScreenPixels(_windowWidth * 0.5f, _windowWidth, 0, _windowHeight * 0.5f, _renderedImage->GetPixels());
	RenderScreenPixels(0, _windowWidth * 0.5f, _windowHeight * 0.5f, _windowHeight, _renderedImage->GetPixels());
	RenderScreenPixels(_windowWidth * 0.5f, _windowWidth, _windowHeight * 0.5f, _windowHeight, _renderedImage->GetPixels());
	Console::WriteFormat("Render took: %f s\n", (float)t.GetElapsedTime());
}

void Engine::SaveRenderedImage()
{
	if(!_renderedImage)
	{
		MessageBox(_windowHandle, "Can't save image cause it wasn't rendered yet", "Are you stupid or something?", MB_OK);
		return;
	}
	
	TGASerializer::SaveTGA("renderedImage.tga", _renderedImage->GetPixels(), _renderedImage->GetWidth(), _renderedImage->GetHeight());
}

void Engine::RenderScreenPixels(int left, int right, int bottom, int top, const Color24* pixels) const
{
	int width = right - left;
	int height = top - bottom;

	uint32* uintpixels = new uint32[_windowWidth * _windowHeight];
	for(int i = 0; i < _windowWidth; ++i)
	{
		for(int j = 0; j < _windowHeight; ++j)
		{
			Color24 pixel = _renderedImage->GetPixel(i, j);
			uintpixels[j * _windowWidth + i] = make(pixel.R * 255, pixel.G * 255, pixel.B * 255, 255);
		}
	}

	BITMAPINFO _bitmapInfo = {};
	_bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	_bitmapInfo.bmiHeader.biWidth = _windowWidth;
	_bitmapInfo.bmiHeader.biHeight = _windowHeight;
	_bitmapInfo.bmiHeader.biPlanes = 1;
	_bitmapInfo.bmiHeader.biBitCount = 32;
	_bitmapInfo.bmiHeader.biCompression = BI_RGB;
	_bitmapInfo.bmiHeader.biSizeImage = 0;
	_bitmapInfo.bmiHeader.biXPelsPerMeter = 0;
	_bitmapInfo.bmiHeader.biYPelsPerMeter = 0;
	_bitmapInfo.bmiHeader.biClrUsed = 0;
	_bitmapInfo.bmiHeader.biClrImportant = 0;
	RECT r;
	GetClientRect(_windowHandle, &r);
	StretchDIBits(GetDC(_windowHandle), left, _windowHeight * 0.5f - bottom, width, height, left, bottom, width, height, uintpixels, &_bitmapInfo, DIB_RGB_COLORS, SRCCOPY);

	delete[] uintpixels;
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
	case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
			case (UINT)MenuCommands::RENDER_IMAGE:
				Engine::GetInstance()->Render();
				break;
			}
		}
		break;
	}
	return DefWindowProc(windowHandle, msg, wParam, lParam);
}
