#pragma once

#include "Vector.h"
#include "Types.h"

class Buffer
{
	friend class DeviceContext;

protected:
	Color32* _pixels;
	int _width;
	int _height;

public:
	Buffer();
	Buffer(int width, int height, uint8 r, uint8 g, uint8 b, uint8 a);
	~Buffer();

	void Clear(uint8 r, uint8 g, uint8 b, uint8 a);
	void SetPixel(int x, int y, uint32 color);

	int GetWidth() const;
	int GetHeight() const;
	const Color32* GetPixels() const;

protected:
	void FillBuffer(uint32 color);
	void Resize(int nWidth, int nHeight);
};