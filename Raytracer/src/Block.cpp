#include "..\include\Block.h"

#include "../include/Defines.h"

#include "Console.h"
#include "../include/Engine.h"

namespace raytracer
{
	Block::Block(int left, int bottom, int width, int height, Color24 backgroundColor) : _x0(left), _y0(bottom), _width(width), _height(height), _backgroundColor(backgroundColor), _renderedImage(0)
	{
		_renderedImage = new Image(width, height, backgroundColor);
	}

	Block::~Block()
	{
		if(_renderedImage)
		{
			delete _renderedImage;
			_renderedImage = 0;
		}
	}

	void Block::Render(const Scene* scene, const Camera* camera)
	{
		if(!scene)
		{
			return;
		}

		auto shapes = scene->GetShapes();
		auto shapesBegin = shapes.begin();
		auto shapesEnd = shapes.end();
		float halfWidth = Engine::GetInstance()->GetWidth() * 0.5f;
		float halfHeight = Engine::GetInstance()->GetHeight() * 0.5f;
		int maxI = _x0 + _width;
		int maxY = _y0 + _height;
		float pixelWidth = Engine::GetInstance()->GetPixelWidth();
		float pixelHeight = Engine::GetInstance()->GetPixelHeight();
		Pixel pixel;
		Ray r;

		for(int i = _x0; i < maxI; ++i)
		{
			for(int j = _y0; j < maxY; ++j)
			{
				pixel.X = -1.0f + (i + 0.5f) * pixelWidth;
				pixel.Y = 1.0f - (j + 0.5f) * pixelHeight;
				pixel.Z = -10.0f;
#if DRAW_GRID
				if((i + _x0) % 80 == 0 || (j + _y0) % 80 == 0)
				{
					_renderedImage->SetPixel(i - _x0, j - _y0, Color24::Yellow);
				}
				else
				{
#endif
					camera->ConstructRay(r, pixel);
					_renderedImage->SetPixel(i - _x0, j - _y0, CastRay(r, shapesBegin, shapesEnd));
#if DRAW_GRID
				}
#endif
			}
		}
	}

	Color24 Block::CastRay(const Ray& r, std::vector<Shape*>::iterator shapesBegin, std::vector<Shape*>::iterator shapesEnd)
	{
		RaycastHit hit;
		Shape* closestShape = nullptr;
		float closestSq = FLT_MAX;
		for(auto shapesIt = shapesBegin; shapesIt != shapesEnd; ++shapesIt)
		{
			if(r.Intersects(*(*shapesIt), hit))
			{
				auto hitPoints = hit.GetIntersectionPoints();
				int hitPointsSize = hitPoints.size();
				for(int k = 0; k < hitPointsSize; ++k)
				{
					float distSq = (hitPoints[k] - r.Origin).LengthSquared();
					if(distSq < closestSq)
					{
						closestSq = distSq;
						closestShape = (*shapesIt);
					}
				}
			}
		}

		if(closestShape)
		{
			return closestShape->GetColor();
		}

		return _backgroundColor;
	}

	Color24 Block::CastRays(const Ray& r, int howManyRays, float pixelWidth, float pixelHeight, std::vector<Shape*>::iterator shapesBegin, std::vector<Shape*>::iterator shapesEnd)
	{
		return _backgroundColor;
	}

	void Block::Clear()
	{
		for(int i = 0; i < _width; ++i)
		{
			for(int j = 0; j < _height; ++j)
			{
				_renderedImage->SetPixel(i, j, _backgroundColor);
			}
		}
	}
}