#include "../include/TGASerializer.h"

#include <stdio.h>
#include <fstream>

void TGASerializer::SaveBuffer(const Buffer* b, const char* filename)
{
	FILE* f = 0;
	fopen_s(&f, filename, "wb");
	if(f == nullptr)
	{
		return;
	}
	
	int width = b->GetWidth();
	int height = b->GetHeight();
	
	putc(0, f); //idLength
	putc(0, f); //colormap
	putc(2, f); //image type = RGB
	putc(0, f); putc(0, f); //colormaporigin
	putc(0, f); putc(0, f); //colormaplength
	putc(0, f); //colormapdepth
	putc(0, f); //xorigin
	putc(0, f); //xorigin
	putc(0, f); //yorigin
	putc(0, f); //yorigin
	putc(width & 0x00FF, f); putc((width & 0xFF00) / 256, f); //width
	putc(height & 0x00FF, f); putc((height & 0xFF00) / 256, f); //height
	putc(32, f); //32 bits per pixel
	putc(0, f); //imagedescriptor
	
	const Color32* pixels = b->GetPixels();

	for(int i = 0; i < width; ++i)
	{
		for(int j = 0; j < height; ++j)
		{
			uint32 rgba = pixels[i * height + j].RGBA();
			putc((rgba & 0xFF000000) >> 24, f);
			putc((rgba & 0x00FF0000) >> 16, f);
			putc((rgba & 0x0000FF00) >> 8, f);
			putc((rgba & 0x000000FF), f);
		}
	}

	fclose(f);
}

void TGASerializer::SaveTGA(const char* filename, const Texture* texture)
{
	FILE* f = 0;
	fopen_s(&f, filename, "wb");
	if(f == nullptr)
	{
		return;
	}

	int width = texture->GetWidth();
	int height = texture->GetHeight();

	putc(0, f); //idLength
	putc(0, f); //colormap
	putc(2, f); //image type = RGB
	putc(0, f); putc(0, f); //colormaporigin
	putc(0, f); putc(0, f); //colormaplength
	putc(0, f); //colormapdepth
	putc(0, f); //xorigin
	putc(0, f); //xorigin
	putc(0, f); //yorigin
	putc(0, f); //yorigin
	putc(width & 0x00FF, f); putc((width & 0xFF00) / 256, f); //width
	putc(height & 0x00FF, f); putc((height & 0xFF00) / 256, f); //height
	putc(32, f); //32 bits per pixel
	putc(0, f); //imagedescriptor

	for(int i = 0; i < width; ++i)
	{
		for(int j = 0; j < height; ++j)
		{
			float u = i / (float)width;
			float v = j / (float)height;
			uint32 rgba = texture->GetPixel(u, v).RGBA();
			putc((rgba & 0x0000FF00) >> 8, f);
			putc((rgba & 0x00FF0000) >> 16, f);
			putc((rgba & 0xFF000000) >> 24, f);
			putc((rgba & 0x000000FF), f);
		}
	}

	fclose(f);
}

void TGASerializer::LoadTGA(const char* filename, Texture** texture)
{
	FILE* f = nullptr;
	fopen_s(&f, filename, "rb");
	if(f == 0)
	{
		return;
	}

	uint16 width;
	uint16 height;
	uint8 tmp;
	tmp = fgetc(f); //idLength
	tmp = fgetc(f); //colormap
	uint8 imageType;
	imageType = fgetc(f); //2 = RGBA
	tmp = fgetc(f); tmp = fgetc(f); //colormaporigin
	tmp = fgetc(f); tmp = fgetc(f); //colormapLength
	tmp = fgetc(f); //colormapDepth
	tmp = fgetc(f); tmp = fgetc(f); //xorigin
	tmp = fgetc(f); tmp = fgetc(f); //yorigin
	uint8 lowByte, highByte;
	lowByte = fgetc(f); highByte = fgetc(f); //width
	width = lowByte + highByte * 256;
	lowByte = fgetc(f); highByte = fgetc(f); //height
	height = lowByte + highByte * 256;
	uint8 bitsPerPixel;
	bitsPerPixel = fgetc(f);
	tmp = fgetc(f); //imageDescriptor

	(*texture) = new Texture(width, height);

	uint8 r, g, b, a;

	a = 255;
	for(int i = 0; i < width; ++i)
	{
		for(int j = 0; j < height; ++j)
		{
			b = fgetc(f);
			g = fgetc(f);
			r = fgetc(f);
			if(bitsPerPixel == 32)
			{
				a = fgetc(f);
			}
			(*texture)->SetPixel(i, j, Color32(r, g, b, a));
		}
	}

	fclose(f);
	f = nullptr;
}
