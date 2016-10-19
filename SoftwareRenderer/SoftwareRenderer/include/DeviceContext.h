#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <vector>

#include "Buffer.h"
#include "DepthBuffer.h"
#include "Vertex.h"
#include "Int3.h"
#include "Rasterizer.h"
#include "Matrix.h"

class DeviceContext
{
	friend class System;

protected:
	Buffer* _backBuffer;
	DepthBuffer* _depthBuffer;
	HWND _windowHandle;
	HINSTANCE _hInstance;
	HDC _deviceContextHandle;
	HBITMAP _bitmapHandle;	//created by CreateDIBSection
	BITMAPINFO _bitmapInfo;
	void* _dibSectionPixels;

	Rasterizer* _rasterizer;

	Matrix _viewMatrix;
	Matrix _perspectiveMatrix;
	Matrix _viewProjectionMatrix;

	Texture* _currentTexture;

	static DeviceContext* _instance;

public:
	DeviceContext();
	~DeviceContext();

	bool Init(HINSTANCE hInstance, LPSTR commandLine, int nCmdShow);
	void Shutdown();
	void SetTexture(Texture* t);
	void DrawIndexed(const Matrix& modelMatrix, const std::vector<VertexPositionUVNormal>& vertices, const std::vector<Int3>& indices, const DirectionalLight* light, const SpotLight* spotlight) const;
	void SwapBuffers();

	void ResizeBuffer(int width, int height);
	void Clear(uint8 r, uint8 g, uint8 b, uint8 a);

	static DeviceContext* GetInstance();
	
protected:
	void Update();

	void PrintScreen();
};

LRESULT CALLBACK WindowsProcedure(HWND windowHandle, UINT msg, WPARAM wParam, LPARAM lParam);