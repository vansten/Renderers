#pragma once

struct Pixel
{
public:
	float X;
	float Y;
	float Z;

	Pixel(float x = 0.0f, float y = 0.0f, float z = 0.0f) : X(x), Y(y), Z(z)
	{}
	Pixel(const Pixel& other) : X(other.X), Y(other.Y), Z(other.Z)
	{}
};