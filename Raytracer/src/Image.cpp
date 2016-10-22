#include "..\include\Image.h"

namespace raytracer
{
	Image::Image(uint32 width, uint32 height, Color24 c) : _width(width), _height(height), _pixels(0)
	{
		_pixels = new Color24[_width * _height];
		for(int i = 0; i < _width; ++i)
		{
			for(int j = 0; j < _height; ++j)
			{
				SetPixel(i, j, c);
			}
		}
	}

	Image::~Image()
	{
		if(_pixels)
		{
			delete[] _pixels;
			_pixels = 0;
		}
	}

	void Image::SetPixel(uint32 x, uint32 y, Color24 color)
	{
		if(x >= 0 && y >= 0 && x < _width && y < _height)
		{
			_pixels[y * _width + x] = color;
		}
	}

	const Color24 Image::GetPixel(uint32 x, uint32 y) const
	{
		if(x >= 0 && y >= 0 && x < _width && y < _height)
		{
			return _pixels[y * _width + x];
		}

		return Color24::Magenta;
	}

	const Color24* Image::GetPixels() const
	{
		return _pixels;
	}
}