#pragma once

#include "Buffer.h"
#include "Texture.h"

class TGASerializer
{
public:
	static void SaveBuffer(const Buffer* b, const char* filename);
	static void SaveTGA(const char* filename, const Texture* texture);
	static void SaveTGA(const char* filename, const Color24* source, int width, int height);
	static void LoadTGA(const char* filename, Texture** texture);
};