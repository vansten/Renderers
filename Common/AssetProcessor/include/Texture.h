#pragma once

#include "Types.h"

enum class TextureWrapMode
{
	Clamp,
	Wrap
};

enum class TextureFiltering
{
	Nearest,
	Bilinear
};

class Texture
{
protected:
	Color32* _pixels;
	int _width;
	int _height;
	TextureWrapMode _wrapMode;
	TextureFiltering _filtering;

public:
	Texture();
	Texture(int width, int height);
	Texture(int width, int height, TextureWrapMode wrapMode);
	Texture(int width, int height, TextureFiltering filtering);
	Texture(int width, int height, TextureWrapMode wrapMode, TextureFiltering filtering);
	~Texture();

	void SetWrapMode(TextureWrapMode wrapMode);
	void SetFiltering(TextureFiltering filtering);
	void SetPixel(int x, int y, Color32 color);
	Color32 GetPixel(float u, float v) const;
	float WrapModeNormalize(float x) const;

	int GetWidth() const;
	int GetHeight() const;
};

