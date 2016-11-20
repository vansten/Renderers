#pragma once

#include "Types.h"

namespace raytracer
{
	class Image
	{
	protected:
		Color24* _pixels;
		uint32 _width;
		uint32 _height;

	public:
		Image(uint32 width, uint32 height, Color24 c = Color24::White);
		~Image();

		void SetPixel(uint32 x, uint32 y, Color24 color);
		inline const Color24 GetPixel(uint32 x, uint32 y) const
		{
			if(x >= 0 && y >= 0 && x < _width && y < _height)
			{
				return _pixels[y * _width + x];
			}

			return Color24::Magenta;
		}
		
		inline const Color24* GetPixels() const
		{
			return _pixels;
		}
		
		inline uint32 GetWidth() const
		{
			return _width;
		}

		inline uint32 GetHeight() const
		{
			return _height;
		}
	};
}