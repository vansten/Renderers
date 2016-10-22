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
		const Color24 GetPixel(uint32 x, uint32 y) const;
		const Color24* GetPixels() const;
		inline uint32 GetWidth()
		{
			return _width;
		}
		inline uint32 GetHeight()
		{
			return _height;
		}
	};
}