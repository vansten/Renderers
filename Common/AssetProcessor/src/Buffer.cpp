#include "../include/Buffer.h"

Buffer::Buffer()
{}


Buffer::Buffer(int width, int height, uint8 r, uint8 g, uint8 b, uint8 a) : _width(width), _height(height)
{
	uint32 color = make(r, g, b, a);
	_pixels = new Color32[_width * _height];
	FillBuffer(color);
}

Buffer::~Buffer()
{
	if(_pixels != 0)
	{
		delete[] _pixels;
		_pixels = 0;
	}
}

void Buffer::Resize(int nWidth, int nHeight)
{
	if(_pixels != 0)
	{
		delete[] _pixels;
		_pixels = 0;
	}

	_pixels = new Color32[nWidth * nHeight];
	_width = nWidth;
	_height = nHeight;
	FillBuffer(0x00000000);
}

void Buffer::Clear(uint8 r, uint8 g, uint8 b, uint8 a)
{
	FillBuffer(make(r,g,b,a));
}

void Buffer::SetPixel(int x, int y, uint32 color)
{
	if(x >= 0 && y >= 0 && x < _width && y < _height)
	{
		_pixels[y * _width + x] = color;
	}
}

int Buffer::GetWidth() const
{
	return _width;
}

int Buffer::GetHeight() const
{
	return _height;
}


const Color32* Buffer::GetPixels() const
{
	return _pixels;
}

void Buffer::FillBuffer(uint32 color)
{
	Color32* pixelStart = _pixels;
	Color32* pixelsEnd = &(_pixels[_width * _height - 1]);
	while(pixelStart <= pixelsEnd)
	{
		*pixelStart = color;
		++pixelStart;
	}
}
