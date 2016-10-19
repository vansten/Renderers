#include "../include/Texture.h"

#include <cmath>

Texture::Texture()
{
	_pixels = 0;
}

Texture::Texture(int width, int height)
{
	_width = width;
	_height = height;
	_pixels = new Color32[_width * _height];
	_filtering = TextureFiltering::Nearest;
	_wrapMode = TextureWrapMode::Wrap;
}

Texture::Texture(int width, int height, TextureWrapMode wrapMode) : Texture(width, height)
{
	_wrapMode = wrapMode;
}

Texture::Texture(int width, int height, TextureFiltering filtering) : Texture(width, height)
{
	_filtering = filtering;
}

Texture::Texture(int width, int height, TextureWrapMode wrapMode, TextureFiltering filtering) : Texture(width, height)
{
	_wrapMode = wrapMode;
	_filtering = _filtering;
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

void Texture::SetFiltering(TextureFiltering filtering)
{
	_filtering = filtering;
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
	else //Wrap texture
	{
		u = WrapModeNormalize(u);
		v = WrapModeNormalize(v);
	}

	if(_filtering == TextureFiltering::Nearest)
	{
		int x = (int)fmax((ceil(u * _width - 1.0f)), 0.0);
		int y = (int)fmax((ceil(v * _height - 1.0f)), 0.0);

		return _pixels[x * _height + y];
	}
	else //Bilinear filtering
	{
		float uWidth = u * _width;
		float vHeight = v * _height;
		int x = int(uWidth);
		int y = int(vHeight);
		float tx = uWidth - x;
		float ty = vHeight - y;
		int x1 = x + 1;
		int y1 = y + 1;
		if(x1 > _width) x1 = x;
		if(y1 > _height) y1 = y;
		const Color32 c00 = _pixels[x * _height + y];
		const Color32 c10 = _pixels[x * _height + y1];
		const Color32 c01 = _pixels[x1 * _height + y];
		const Color32 c11 = _pixels[x1 * _height + y1];
		Color32 a = c00 * (1.f - tx) + c10 * tx;
		Color32 b = c01 * (1.f - tx) + c11 * tx;
		return a * (1.f - ty) + b * ty;
	}
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
