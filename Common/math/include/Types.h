#pragma once

#include <stdint.h>

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;

uint32 make(uint8 r, uint8 g, uint8 b, uint8 a);
void getRGBA(uint32 color, uint8& r, uint8& g, uint8& b, uint8& a);

struct Color32
{
public:
	union
	{
		uint32 Value;
		struct
		{
			uint8 B;
			uint8 G;
			uint8 R;
			uint8 A;
		};
	};

public:
	static Color32 Black;
	static Color32 Red;
	static Color32 Green;
	static Color32 Blue;
	static Color32 Yellow;
	static Color32 Magenta;
	static Color32 Cyan;
	static Color32 White;

public:
	Color32() : Value(0) {}
	Color32(uint8 r, uint8 g, uint8 b, uint8 a) : Value(0) 
	{
		Value = make(r, g, b, a);
	}
	Color32(const Color32& col) : Value(col) {}

	uint32 RGBA() const
	{
		uint8 r = (Value & 0x00FF0000) >> 16;
		uint8 g = (Value & 0x0000FF00) >> 8;
		uint8 b = (Value & 0x000000FF);
		uint8 a = (Value & 0xFF000000) >> 24;

		return (r << 24) | (g << 16) | (b << 8) | (a);
	}

	operator uint32() const
	{
		return Value;
	}

	void operator=(uint32 value)
	{
		Value = value;
	}

	Color32 operator*(float s) const
	{
		Color32 c = *this;
		c.R = (uint8)(c.R * s);
		c.G = (uint8)(c.G * s);
		c.B = (uint8)(c.B * s);
		c.A = (uint8)(c.A * s);
		return c;
	}

	Color32 operator*(Color32 c) const
	{
		Color32 me = *this;
		me.R = (uint8)(me.R * (float)(c.R / 255.0f));
		me.G = (uint8)(me.G * (float)(c.G / 255.0f));
		me.B = (uint8)(me.B * (float)(c.B / 255.0f));
		me.A = (uint8)(me.A * (float)(c.A / 255.0f));

		return me;
	}

	Color32 operator+(const Color32& other) const
	{
		Color32 c = *this;
		c.R += other.R;
		c.G += other.G;
		c.B += other.B;
		c.A += other.A;
		return c;
	}
};