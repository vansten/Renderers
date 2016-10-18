#include "../include/Texture.h"

#include <cmath>

Texture::Texture()
{
	_pixels = 0;
}

Texture::Texture(int width, int height, TextureWrapMode wrapMode)
{
	_width = width;
	_height = height;
	_pixels = new Color32[_width * _height];
	_wrapMode = wrapMode;
}

Texture::~Texture()
{
	if(_pixels)
	{
		delete[] _pixels;
		_pixels = 0;
	}
}

void Texture::SetWrapMode(TextureWrapMode wrapMode)
{
	_wrapMode = wrapMode;
}

void Texture::SetPixel(int x, int y, Color32 color)
{
	if(x >= 0 && y >= 0 && x < _width && y < _height)
	{
		_pixels[y * _width + x] = color;
	}
}

Color32 Texture::GetPixel(float u, float v) const
{
	if(_wrapMode == TextureWrapMode::Clamp)
	{
		u = fmax(0.0f, fmin(u, 1.0f));
		v = fmax(0.0f, fmin(v, 1.0f));
	}
	else
	{
		u = WrapModeNormalize(u);
		v = WrapModeNormalize(v);
	}

	int x = u * _width;
	int y = v * _height;
	return _pixels[y * _width + x];
}

float Texture::WrapModeNormalize(float x) const
{
	float r = x;
	while(r < 0.0f)
	{
		r += 1.0f;
	}
	while(r > 1.0f)
	{
		r -= 1.0f;
	}
	return r;
}

int Texture::GetWidth() const
{
	return _width;
}

int Texture::GetHeight() const
{
	return _height;
}
