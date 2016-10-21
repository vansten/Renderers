#include "../include/Engine.h"
#include "../include/Scene.h"
#include "../include/Console.h"
#include "../include/Image.h"
#include "../include/TGASerializer.h"
#include "../include/Timer.h"
#include "../include/Block.h"
#include "../include/Defines.h"

#include <thread>

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
#if USE_CONSOLE
	Console::Open();
#endif
	
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

#if BLOCKS
	int blockCount = 4;
	if(blockCount <= 0)
	{
		Console::WriteLine("Block count cannot be less or equal to 0");
		return false;
	}
	else if(blockCount % 2 != 0)
	{
		Console::WriteLine("Block count should be divisible by 2");
		return false;
	}

	int x = blockCount / 2;
	int y = x;
	int widthPerBlock = _windowWidth / x;
	int heightPerBlock = _windowHeight / y;

	for(int i = 0; i < x; ++i)
	{
		for(int j = 0; j < y; ++j)
		{
			_blocks.push_back(new Block(i * widthPerBlock, j * heightPerBlock, widthPerBlock, heightPerBlock, Color24::White * 0.3f));
		}
	}
#endif

	return true;
}

void Engine::Shutdown()
{
	auto blocksIt = _blocks.begin();
	auto blocksEnd = _blocks.end();
	for(blocksIt; blocksIt != blocksEnd; ++blocksIt)
	{
		delete (*blocksIt);
	}

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

#if USE_CONSOLE
	system("pause");
	Console::Close();
#endif
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
	if(_renderedImage)
	{
		delete _renderedImage;
		_renderedImage = 0;
	}

	Timer t;
	t.Start();
#if BLOCKS
	auto blocksIt = _blocks.begin();
	auto blocksEnd = _blocks.end();

#if MULTITHREADED
	std::vector<std::thread> threads;
	for(blocksIt; blocksIt != blocksEnd; ++blocksIt)
	{
		threads.push_back(std::thread(&Engine::RenderBlock, this, (*blocksIt)));
	}

	auto threadIt = threads.begin();
	auto threadEnd = threads.end();
	for(threadIt; threadIt != threadEnd; ++threadIt)
	{
		(*threadIt).join();
	}
#else
	for(blocksIt; blocksIt != blocksEnd; ++blocksIt)
	{
		(*blocksIt)->Clear();
		(*blocksIt)->Render(_scene);
		PresentBlockWork((*blocksIt));
	}
#endif //MULTITHREADED
#else
	_renderedImage = new Image(_windowWidth, _windowHeight, Color24::White * 0.3f);

	_scene->Render(_renderedImage);

	RenderScreenPixels(0, _windowWidth * 0.5f, 0, _windowHeight * 0.5f, _renderedImage->GetPixels());
	RenderScreenPixels(_windowWidth * 0.5f, _windowWidth, 0, _windowHeight * 0.5f, _renderedImage->GetPixels());
	RenderScreenPixels(0, _windowWidth * 0.5f, _windowHeight * 0.5f, _windowHeight, _renderedImage->GetPixels());
	RenderScreenPixels(_windowWidth * 0.5f, _windowWidth, _windowHeight * 0.5f, _windowHeight, _renderedImage->GetPixels());

#endif //BLOCKS

	float renderTime = t.GetElapsedTime();
	Console::WriteFormat("Render took: %f s\n", renderTime);
}

void Engine::RenderBlock(Block* b) const
{
	b->Clear();
	b->Render(_scene);
	PresentBlockWork(b);
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
	StretchDIBits(GetDC(_windowHandle),
				  left, _windowHeight * 0.5f - bottom, width, height,
				  left, bottom, width, height,
				  uintpixels,
				  &_bitmapInfo, DIB_RGB_COLORS, SRCCOPY);

	delete[] uintpixels;
}

void Engine::PresentBlockWork(const Block* block) const
{
	if(!block)
	{
		return;
	}

	int width = block->GetWidth();
	int height = block->GetHeight();
	const Color24* pixels = block->GetPixels();

	uint32* uintpixels = new uint32[width * height];
	for(int i = 0; i < width; ++i)
	{
		for(int j = 0; j < height; ++j)
		{
			Color24 pixel = pixels[j * width + i];
			uintpixels[j * width + i] = make(pixel.R * 255, pixel.G * 255, pixel.B * 255, 255);
		}
	}

	BITMAPINFO _bitmapInfo = {};
	_bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	_bitmapInfo.bmiHeader.biWidth = width;
	_bitmapInfo.bmiHeader.biHeight = height;
	_bitmapInfo.bmiHeader.biPlanes = 1;
	_bitmapInfo.bmiHeader.biBitCount = 32;
	_bitmapInfo.bmiHeader.biCompression = BI_RGB;
	_bitmapInfo.bmiHeader.biSizeImage = 0;
	_bitmapInfo.bmiHeader.biXPelsPerMeter = 0;
	_bitmapInfo.bmiHeader.biYPelsPerMeter = 0;
	_bitmapInfo.bmiHeader.biClrUsed = 0;
	_bitmapInfo.bmiHeader.biClrImportant = 0;
	StretchDIBits(GetDC(_windowHandle),
				  block->GetX(), _windowHeight - height - block->GetY(), width, height,
				  0, 0, width, height,
				  uintpixels, 
				  &_bitmapInfo, DIB_RGB_COLORS, SRCCOPY);

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
