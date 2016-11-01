#define _CRT_SECURE_NO_WARNINGS

#include "../include/Engine.h"
#include "../include/Scene.h"
#include "../include/Console.h"
#include "../include/Image.h"
#include "../include/TGASerializer.h"
#include "../include/Timer.h"
#include "../include/Block.h"
#include "../include/Defines.h"

#include <thread>

#include <ShlObj.h>

namespace raytracer
{
	Engine* Engine::_instance = 0;

	Engine::Engine() : _scene(0), _renderedImage(0), _camera(0)
	{
		_instance = this;
	}


	Engine::~Engine()
	{}

	bool Engine::Init(HINSTANCE hInstance, LPSTR commandLine, int nCmdShow)
	{
#if USE_CONSOLE
		Console::Open();
#endif
		_pixelWidth = 2.0f / (float)_windowWidth;
		_pixelHeight = 2.0f / (float)_windowHeight;

		WNDCLASS windowClass = {};
		windowClass.hInstance = hInstance;
		windowClass.lpfnWndProc = WindowsProcedure;
		windowClass.lpszClassName = "vanstenRaytracerWindowClass";
		windowClass.style = CS_VREDRAW | CS_HREDRAW;

		if(!RegisterClass(&windowClass))
		{
			Console::WriteLine("Can't register window class\n");
			return false;
		}

		_windowHandle = CreateWindow(windowClass.lpszClassName, "vansten Raytracer", WS_OVERLAPPEDWINDOW, 100, 100, _windowWidth, _windowHeight, 0, 0, hInstance, 0);
		if(!_windowHandle)
		{
			Console::WriteLine("Couldn't create window");
			return false;
		}

		_menuHandle = CreateMenu();
		AppendMenu(_menuHandle, MF_STRING, (UINT)MenuCommands::RENDER_IMAGE, "Render");
		AppendMenu(_menuHandle, MF_STRING, (UINT)MenuCommands::SAVE_IMAGE, "Save image");
		SetMenu(_windowHandle, _menuHandle);
		EnableMenuItem(_menuHandle, (UINT)MenuCommands::SAVE_IMAGE, MF_DISABLED);
		
		ShowWindow(_windowHandle, nCmdShow);

		Image* i = new Image(_windowWidth, _windowHeight, Color24::Black);
		RenderScreenPixels(0, _windowWidth, 0, _windowHeight, i->GetPixels());
		delete i;

		if(!SetupScene())
		{
			Console::WriteLine("Couldn't setup scene");
			return false;
		}

#if BLOCKS

#if MULTITHREADED
#if PC
		_threadCount = 4;
#else
		_threadCount = 8;
#endif
#else
		_threadCount = 1;
#endif

		int blockRows = 4;
		int blockCount = blockRows * blockRows;
		blockCount = max(_threadCount, blockCount);
		if(blockCount % 2 != 0)
		{
			Console::WriteLine("Block count should be divisible by 2");
			return false;
		}

		int widthPerBlock = _windowWidth / blockRows;
		int heightPerBlock = _windowHeight / blockRows;
		int usedWidth = 0;
		int usedHeight = 0;

		for(int i = 0; i < blockRows; ++i)
		{
			for(int j = 0; j < blockRows; ++j)
			{
				_blocks.push_back(new Block(i * widthPerBlock, j * heightPerBlock, min(_windowWidth - usedWidth, widthPerBlock), min(_windowHeight - usedHeight, heightPerBlock), Color24::White * 0.1f));
				usedHeight += heightPerBlock;
			}
			usedWidth += widthPerBlock;
			usedHeight = 0;
		}
#endif

#if ORTHO
		_camera = new OrthoCamera(ORTHO_SIZE, -10.0f);
#else
		_camera = new PerspectiveCamera(60.0f, 0.001f, 1000.0f);
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

		if(_camera)
		{
			delete _camera;
			_camera = 0;
		}

		if(_scene)
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
		while(_bRunning)
		{
			while(PeekMessage(&msg, _windowHandle, 0, 0, PM_REMOVE))
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

		_renderedImage = new Image(_windowWidth, _windowHeight, Color24::Black);
		if(_renderedImage)
		{
			RenderScreenPixels(0, _windowWidth, 0, _windowHeight, _renderedImage->GetPixels());
			delete _renderedImage;
			_renderedImage = 0;
		}

		_renderedImage = new Image(_windowWidth, _windowHeight, Color24::Black);

		Timer t;
		t.Start();
#if BLOCKS

		int blocksPerThread = _blocks.size() / _threadCount;
		std::vector<std::thread> threads;
		int start = 0;
		for(int i = 0; i < _threadCount - 1; ++i)
		{
			threads.push_back(std::thread(&Engine::RenderBlocks, this, start, blocksPerThread));
			start += blocksPerThread;
		}

		RenderBlocks(start, blocksPerThread);

		auto threadsIt = threads.begin();
		auto threadsEnd = threads.end();
		for(threadsIt; threadsIt != threadsEnd; ++threadsIt)
		{
			(*threadsIt).join();
		}
#else
		_renderedImage = new Image(_windowWidth, _windowHeight, Color24::Black);

		_scene->Render(_renderedImage);
		RenderScreenPixels(0, _windowWidth, 0, _windowHeight, _renderedImage->GetPixels());

#endif //BLOCKS

		float renderTime = t.GetElapsedTime();
		Console::WriteFormat("Render took: %f s\n", renderTime);

		int response = MessageBox(_windowHandle, "Rendering completed", "Success", MB_OK);
		EnableMenuItem(_menuHandle, (UINT)MenuCommands::SAVE_IMAGE, MF_ENABLED);
	}

	void Engine::RenderBlock(Block* b) const
	{
		b->Clear();
		b->Render(_scene, _camera);
		PresentBlockWork(b);
	}

	void Engine::RenderBlocks(const int start, const int howMany) const
	{
		int max = min(start + howMany, _blocks.size());
		for(int i = start; i < max; ++i)
		{
			RenderBlock(_blocks.at(i));
		}

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
				Color24 pixel = pixels[j * _windowWidth + i];
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
					  left, bottom, width, height,
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
				_renderedImage->SetPixel(i + block->GetX(), j + block->GetY(), pixel);
			}
		}

		static BITMAPINFO bitmapInfo = {
											sizeof(BITMAPINFOHEADER), //size
											width,	//width
											height, //height
											1, //planes
											32, //bits per pixel
											BI_RGB,  //compresion
											0, //sizeimage
											0, //xpelspermeter
											0, //ypelspermeter
											0, //clrused
											0 //clrimportant
		};
		StretchDIBits(GetDC(_windowHandle),
					  block->GetX(), _windowHeight - block->GetY() - height, width, height,
					  0, 0, width, height,
					  uintpixels,
					  &bitmapInfo, DIB_RGB_COLORS, SRCCOPY);

		delete[] uintpixels;
	}

	void Engine::SaveImage() const
	{
		if(_renderedImage == nullptr)
		{
			MessageBox(_windowHandle, "You must render image first", "Oops", MB_OK);
			return;
		}

		IFileSaveDialog* saveFileDialog;
		if(SUCCEEDED(CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&saveFileDialog))))
		{
			COMDLG_FILTERSPEC rgSpec[] =
			{
				{L"TGA file", L"*.tga;*.TGA"}
			};
			saveFileDialog->SetFileTypes(1, rgSpec);
			if(SUCCEEDED(saveFileDialog->Show(_windowHandle)))
			{
				IShellItem* chosenItem;
				saveFileDialog->GetResult(&chosenItem);
				if(chosenItem != nullptr)
				{
					const int maxLength = 500;
					LPWSTR filename;
					chosenItem->GetDisplayName(SIGDN_FILESYSPATH, &filename);
					char* filenameChar = new char[maxLength];
					wcstombs(filenameChar, filename, maxLength);
					int dotIndex = -1;
					int lastChar = -1;
					for(int i = 0; i < maxLength; ++i)
					{
						if(filenameChar[i] == '.')
						{
							dotIndex = i;
						}
						else if(filenameChar[i] == '\0')
						{
							lastChar = i;
						}
					}
					if(dotIndex == -1)
					{
						char buffer[maxLength + 5];
						memcpy(buffer, filenameChar, sizeof(char) * (maxLength));
						buffer[lastChar] = '.';
						buffer[lastChar + 1] = 't';
						buffer[lastChar + 2] = 'g';
						buffer[lastChar + 3] = 'a';
						buffer[lastChar + 4] = '\0';
						delete[] filenameChar;
						filenameChar = new char[lastChar + 5];
						memcpy(filenameChar, buffer, sizeof(char) * (lastChar + 5));
					}
					else
					{
						if(filenameChar[dotIndex + 1] != 't' || filenameChar[dotIndex + 2] != 'g' || filenameChar[dotIndex + 3] != 'a')
						{
							filenameChar[dotIndex + 1] = 't';
							filenameChar[dotIndex + 2] = 'g';
							filenameChar[dotIndex + 3] = 'a';
							filenameChar[dotIndex + 4] = '\0';
						}
					}
					TGASerializer::SaveTGA(filenameChar, _renderedImage->GetPixels(), _windowWidth, _windowHeight);
					delete[] filenameChar;
				}
			}
		}
	}

	Engine* Engine::GetInstance()
	{
		return _instance;
	}

	bool Engine::SetupScene()
	{
		_scene = new Scene();
		if(!_scene)
		{
			return false;
		}
		_scene->Init();

		return true;
	}

	LRESULT CALLBACK WindowsProcedure(HWND windowHandle, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch(msg)
		{
		case WM_QUIT:
		case WM_CLOSE:
		case WM_DESTROY:
			Engine::GetInstance()->Exit();
			break;
		case WM_KEYUP:
			if(wParam == VK_ESCAPE)
			{
				Engine::GetInstance()->Exit();
			}
			else if(wParam == VK_F5)
			{
				Engine::GetInstance()->Render();
			}
			break;
		case WM_COMMAND:
			{
				switch(LOWORD(wParam))
				{
				case (UINT)MenuCommands::RENDER_IMAGE:
					Engine::GetInstance()->Render();
					break;
				case (UINT)MenuCommands::SAVE_IMAGE:
					Engine::GetInstance()->SaveImage();
					break;
				}
			}
			break;
		}
		return DefWindowProc(windowHandle, msg, wParam, lParam);
	}
}