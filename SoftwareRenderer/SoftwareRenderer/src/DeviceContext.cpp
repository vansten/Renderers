#include "../include/DeviceContext.h"
#include "../include/System.h"
#include "TGASerializer.h"
#include "Console.h"

DeviceContext* DeviceContext::_instance = 0;

DeviceContext::DeviceContext() : _backBuffer(0), _windowHandle(0), _hInstance(0), _deviceContextHandle(0)
{
	_instance = this;
}

DeviceContext::~DeviceContext()
{
	_instance = 0;
}

bool DeviceContext::Init(HINSTANCE hInstance, LPSTR commandLine, int nCmdShow)
{
	int windowWidth = 640;
	int windowHeight = 480;

	WNDCLASS windowClass = {};
	windowClass.hInstance = hInstance;
	windowClass.lpfnWndProc = WindowsProcedure;
	windowClass.lpszClassName = "DTGLClassName";
	windowClass.style = CS_VREDRAW | CS_HREDRAW;

	RegisterClass(&windowClass);
	_windowHandle = CreateWindow(windowClass.lpszClassName, "DT GL", WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU, 960 - windowWidth * 0.5f, 540 - windowHeight * 0.5f, windowWidth, windowHeight, 0, 0, hInstance, 0);
	if(!_windowHandle)
	{
		return false;
	}

	_backBuffer = new Buffer(windowWidth, windowHeight, 0xFF, 0x00, 0x00, 0xFF);
	_depthBuffer = new DepthBuffer(windowWidth, windowHeight);
	ShowWindow(_windowHandle, nCmdShow);

	_deviceContextHandle = GetDC(_windowHandle);
	if(!_deviceContextHandle)
	{
		return false;
	}

	_bitmapInfo = {};
	_bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	_bitmapInfo.bmiHeader.biWidth = windowWidth;
	_bitmapInfo.bmiHeader.biHeight = windowHeight;
	_bitmapInfo.bmiHeader.biPlanes = 1;
	_bitmapInfo.bmiHeader.biBitCount = 32;
	_bitmapInfo.bmiHeader.biCompression = BI_RGB;
	_bitmapInfo.bmiHeader.biSizeImage = 0;
	_bitmapInfo.bmiHeader.biXPelsPerMeter = 0;
	_bitmapInfo.bmiHeader.biYPelsPerMeter = 0;
	_bitmapInfo.bmiHeader.biClrUsed = 0;
	_bitmapInfo.bmiHeader.biClrImportant = 0;
	_bitmapHandle = CreateDIBSection(_deviceContextHandle, &_bitmapInfo, DIB_RGB_COLORS, &_dibSectionPixels, 0, 0);

	if(!_bitmapHandle)
	{
		return false;
	}

	_rasterizer = new Rasterizer();
	if(!_rasterizer)
	{
		return false;
	}

	float fov = 45.0f;
	float aspectRatio = (float)windowWidth / (float)windowHeight;
	float zNear = 0.01f;
	float zFar = 1000.0f;
	Vector3 eyePosition(0, 0, -5);
	Vector3 target;
	Vector3 up(0, 1, 0);

	_viewMatrix = Matrix::LookAt(eyePosition, target, up);
	_perspectiveMatrix = Matrix::PerspectiveLHDeg(fov, aspectRatio, zNear, zFar);
	_viewProjectionMatrix = _perspectiveMatrix.Multiply(_viewMatrix);

	return true;
}

void DeviceContext::Shutdown()
{
	if(_backBuffer)
	{
		delete _backBuffer;
		_backBuffer = 0;
	}

	if(_depthBuffer)
	{
		delete _depthBuffer;
		_depthBuffer = 0;
	}

	if(_bitmapHandle)
	{
		DeleteObject(_bitmapHandle);
	}

	if(_deviceContextHandle)
	{
		ReleaseDC(_windowHandle, _deviceContextHandle);
		DeleteObject(_deviceContextHandle);
	}

	if(_windowHandle)
	{
		CloseWindow(_windowHandle);
		DestroyWindow(_windowHandle);
	}

	if(_rasterizer)
	{
		delete _rasterizer;
		_rasterizer = 0;
	}
}

void DeviceContext::SetTexture(Texture* t)
{
	_currentTexture = t;
}

void DeviceContext::DrawIndexed(const Matrix& modelMatrix, const std::vector<VertexPositionUVNormal>& vertices, const std::vector<Int3>& indices, DirectionalLight* light)
{
	uint32 indicesSize = (uint32)indices.size();
	Matrix mvp = _viewProjectionMatrix.Multiply(modelMatrix);
	for(uint32 i = 0; i < indicesSize; ++i)
	{
		Int3 indexThree = indices[i];
		VertexPositionUVNormal v1 = vertices[indexThree[0]];
		VertexPositionUVNormal v2 = vertices[indexThree[1]];
		VertexPositionUVNormal v3 = vertices[indexThree[2]];
		_rasterizer->DrawTriangleWithTexture(_backBuffer, _depthBuffer, modelMatrix, mvp, 
								  v1, v2, v3,
								  _currentTexture,
								  light);
	}
}

void DeviceContext::SwapBuffers()
{
	int w = _backBuffer->_width;
	int h = _backBuffer->_height;

	RECT r;
	GetClientRect(_windowHandle, &r);
	StretchDIBits(_deviceContextHandle, 0, 0, w, h, 0, 0, w, h, (uint32*)_backBuffer->_pixels, &_bitmapInfo, DIB_RGB_COLORS, SRCCOPY);
}

void DeviceContext::ResizeBuffer(int width, int height)
{
	_backBuffer->Resize(width, height);
}

void DeviceContext::Clear(uint8 r, uint8 g, uint8 b, uint8 a)
{
	_backBuffer->Clear(r, g, b, a);
	_depthBuffer->Clear();
}

DeviceContext* DeviceContext::GetInstance()
{
	return _instance;
}

void DeviceContext::Update()
{
	MSG msg;
	while(PeekMessage(&msg, _windowHandle, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void DeviceContext::PrintScreen()
{
	TGASerializer::SaveBuffer(_backBuffer, "printscreen.tga");
	Console::WriteLine("Print screen done. Check printscreen.tga file");
}

LRESULT CALLBACK WindowsProcedure(HWND windowHandle, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_CLOSE:
	case WM_DESTROY:
	case WM_QUIT:
		System::bRunning = false;
		break;
	case WM_KEYUP:
		if(wParam == VK_ESCAPE)
		{
			System::bRunning = false;
		}
		else if(wParam == VK_F2)
		{
			//System::GetInstance()->MakePrintScreen();
		}
		break;
	default:
		return DefWindowProc(windowHandle, msg, wParam, lParam);
	}

	return DefWindowProc(windowHandle, msg, wParam, lParam);
}
