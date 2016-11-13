#pragma once

#include <stdint.h>
#include <cmath>

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;

uint32 make(uint8 r, uint8 g, uint8 b, uint8 a);
void getRGBA(uint32 color, uint8& r, uint8& g, uint8& b, uint8& a);
float clamp(const float& value, const float min = 0, const float max = 1);

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
	static const Color32 Black;
	static const Color32 Red;
	static const Color32 Green;
	static const Color32 Blue;
	static const Color32 Yellow;
	static const Color32 Magenta;
	static const Color32 Cyan;
	static const Color32 White;

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
		c.R = (uint8)clamp(((uint32)c.R) * s, 0, 255);
		c.G = (uint8)clamp(((uint32)c.G) * s, 0, 255);
		c.B = (uint8)clamp(((uint32)c.B) * s, 0, 255);
		c.A = (uint8)clamp(((uint32)c.A) * s, 0, 255);
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

struct Color24
{
public:
	static const Color24 Black;
	static const Color24 Red;
	static const Color24 Green;
	static const Color24 Blue;
	static const Color24 Yellow;
	static const Color24 Magenta;
	static const Color24 Cyan;
	static const Color24 White;

public:
	float R;
	float G;
	float B;

	Color24(float r = 0.0f, float g = 0.0f, float b = 0.0f) : R(clamp(r)), G(clamp(g)), B(clamp(b)) {}
	Color24(const Color24& other) : R(other.R), G(other.G), B(other.B) {}
	Color24(const Color32& other) : R(other.R / 255.0f), G(other.G / 255.0f), B(other.B / 255.0f) {}

	Color24 operator+(const Color24& c) const
	{
		return Color24(R + c.R, G + c.G, B + c.B);
	}

	Color24 operator-(const Color24& c) const
	{
		return Color24(fabs(R - c.R), fabs(G - c.G), fabs(B - c.B));
	}

	void operator+=(const Color24& c)
	{
		R = clamp(R + c.R);
		G = clamp(G + c.G);
		B = clamp(B + c.B);
	}

	void operator-=(const Color24& c)
	{
		R = clamp(R - c.R);
		G = clamp(G - c.G);
		B = clamp(B - c.B);
	}

	Color24 operator*(const float s) const
	{
		return Color24(R * s, G * s, B * s);
	}

	Color24 operator*(const Color24& c) const
	{
		return Color24(R * c.R, G * c.G, B * c.B);
	}

	Color24 operator/(const float s) const
	{
		if(s == 0.0f)
		{
			return Color24();
		}
		return (*this) * (1.0f / s);
	}

	void operator*=(const float s)
	{
		R = clamp(R * s);
		G = clamp(G * s);
		B = clamp(B * s);
	}

	void operator*=(const Color24& c)
	{
		R = clamp(R * c.R);
		G = clamp(G * c.G);
		B = clamp(B * c.B);
	}

	void operator/=(const float s)
	{
		(*this) *= (1.0f / s);
	}

	friend Color24 operator*(const float s, const Color24& c);
};