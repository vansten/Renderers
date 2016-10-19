#include "../include/Types.h"

uint32 make(uint8 r, uint8 g, uint8 b, uint8 a)
{
	return (a << 24) | (r << 16) | (g << 8) | (b);
}

void getRGBA(uint32 color, uint8& r, uint8& g, uint8& b, uint8& a)
{
	b = (color & 0x000000ff);
	g = (color & 0x0000ff00) >> 8;
	r = (color & 0x00ff0000) >> 16;
	a = (color & 0xff000000) >> 24;
}

float clamp(const float & value, const float min, const float max)
{
	return fmax(fmin(value, max), min);
}

Color32 Color32::Black = Color32(0x00, 0x00, 0x00, 0x00);
Color32 Color32::Red = Color32(0xFF, 0x00, 0x00, 0xFF);
Color32 Color32::Green = Color32(0x00, 0xFF, 0x00, 0xFF);
Color32 Color32::Blue = Color32(0x00, 0x00, 0xFF, 0xFF);
Color32 Color32::Yellow = Color32(0xFF, 0xFF, 0x00, 0xFF);
Color32 Color32::Magenta = Color32(0xFF, 0x00, 0xFF, 0xFF);
Color32 Color32::Cyan = Color32(0x00, 0xFF, 0xFF, 0xFF);
Color32 Color32::White = Color32(0xFF, 0xFF, 0xFF, 0xFF);