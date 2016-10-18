#pragma once

#include "Types.h"

enum class TextureWrapMode
{
	Clamp,
	Wrap
};

class Texture
{
protected:
	Color32* _pixels;
	int _width;
	int _height;
	TextureWrapMode _wrapMode;

public:
	Texture();
	Texture(int width, int height, TextureWrapMode wrapMode = TextureWrapMode::Wrap);
	~Texture();

	void SetWrapMode(TextureWrapMode wrapMode);
	void SetPixel(int x, int y, Color32 color);
	Color32 GetPixel(float u, float v) const;
	float WrapModeNormalize(float x) const;

	int GetWidth() const;
	int GetHeight() const;
};

