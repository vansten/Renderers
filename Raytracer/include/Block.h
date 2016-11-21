#pragma once

#include "../include/Image.h"
#include "../include/Scene.h"
#include "../include/Pixel.h"

namespace raytracer
{
	class Camera;

	class Block
	{
	protected:
		Image* _renderedImage;

		Color24 _backgroundColor;
		int _x0;
		int _y0;
		int _width;
		int _height;

	public:
		Block(int left, int bottom, int width, int height, Color24 backgroundColor = Color24::White);
		~Block();
		void Render(const Scene* scene, const Camera* camera);
		Color24 CastRay(const Ray& r, std::vector<Shape*>::iterator shapesBegin, std::vector<Shape*>::iterator shapesEnd, std::vector<Light*>::iterator lightsBegin, std::vector<Light*>::iterator lightsEnd, int steps);
		Color24 CastRays(const Pixel& center, int maxSteps, float pixelWidth, float pixelHeight, std::vector<Shape*>::iterator shapesBegin, std::vector<Shape*>::iterator shapesEnd, const Camera* camera, std::vector<Light*>::iterator lightsBegin, std::vector<Light*>::iterator lightsEnd);
		void Clear();

		inline int GetX() const
		{
			return _x0;
		}

		inline int GetY() const
		{
			return _y0;
		}

		inline int GetWidth() const
		{
			return _width;
		}

		inline int GetHeight() const
		{
			return _height;
		}

		inline const Color24* GetPixels() const
		{
			if(_renderedImage)
			{
				return _renderedImage->GetPixels();
			}
			return nullptr;
		}

		inline const Image* GetRenderedImage() const
		{
			return _renderedImage;
		}
	};
}