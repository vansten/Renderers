#include "../include/DepthBuffer.h"
#include <cfloat>

DepthBuffer::DepthBuffer(int width, int height)
{
	_width = width;
	_height = height;
	_depth = new float[width * height];
	Clear();
}

DepthBuffer::~DepthBuffer()
{
	delete[] _depth;
	_depth = 0;
}

void DepthBuffer::Clear()
{
	for(int i = 0; i < _width; ++i)
	{
		for(int j = 0; j < _height; ++j)
		{
			SetDepth(i, j, FLT_MAX);
		}
	}
}

float DepthBuffer::GetDepth(int x, int y)
{
	return _depth[y * _width + x];
}

void DepthBuffer::SetDepth(int x, int y, float value)
{
	_depth[y * _width + x] = value;
}
