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
				pixel.Y = -1.0f + (j + 0.5f) * pixelHeight;
				pixel.Z = -10.0f;
#if DRAW_GRID
				if((i + _x0) % 80 == 0 || (j + _y0) % 80 == 0)
				{
					_renderedImage->SetPixel(i - _x0, j - _y0, Color24::Yellow);
				}
				else
				{
#endif
#if ANTI_ALIASING
					_renderedImage->SetPixel(i - _x0, j - _y0, CastRays(pixel, MAX_STEPS, pixelWidth, pixelHeight, shapesBegin, shapesEnd, camera));
#else
					camera->ConstructRay(r, pixel);
					_renderedImage->SetPixel(i - _x0, j - _y0, CastRay(r, shapesBegin, shapesEnd));
#endif
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

	Color24 Block::CastRays(const Pixel& center, int maxSteps, float pixelWidth, float pixelHeight, std::vector<Shape*>::iterator shapesBegin, std::vector<Shape*>::iterator shapesEnd, const Camera* camera)
	{
		if(maxSteps == 0)
		{
			Ray r;
			camera->ConstructRay(r, center);
			return CastRay(r, shapesBegin, shapesEnd);
		}

		float halfPixelW = 0.5f * pixelWidth;
		float halfPixelH = 0.5f * pixelHeight;
		Pixel corners[4] = {center, center, center, center};
		
		corners[0].X -= halfPixelW;
		corners[0].Y -= halfPixelH;

		corners[1].X -= halfPixelW;
		corners[1].Y += halfPixelH;
		
		corners[2].X += halfPixelW;
		corners[2].Y -= halfPixelH;
		
		corners[3].X += halfPixelW;
		corners[3].Y += halfPixelH;

		Ray rays[4];
		Color24 colors[4];
		for(int i = 0; i < 4; ++i)
		{
			camera->ConstructRay(rays[i], corners[i]);
			colors[i] = CastRay(rays[i], shapesBegin, shapesEnd);
		}

		Color24 diff(0, 0, 0);
		for(int i = 0; i < 3; ++i)
		{
			for(int j = i + 1; j < 4; ++j)
			{
				diff += colors[j] - colors[i];
			}
		}

		if(diff.R > 0.05f || diff.G > 0.05f || diff.B > 0.05f)
		{
			Pixel subCenters[4];
			subCenters[0].X = center.X - halfPixelW * 0.25f;
			subCenters[0].Y = center.Y - halfPixelH * 0.25f;

			subCenters[1].X = center.X - halfPixelW * 0.25f;
			subCenters[1].Y = center.Y + halfPixelH * 0.25f;

			subCenters[2].X = center.X + halfPixelW * 0.25f;
			subCenters[2].Y = center.Y - halfPixelH * 0.25f;

			subCenters[3].X = center.X + halfPixelW * 0.25f;
			subCenters[3].Y = center.Y + halfPixelH * 0.25f;

			for(int i = 0; i < 4; ++i)
			{
				colors[i] = CastRays(subCenters[i], maxSteps - 1, halfPixelW, halfPixelH, shapesBegin, shapesEnd, camera);
			}
		}

		return 0.25f * colors[0] + 0.25f * colors[1] + 0.25f * colors[2] + 0.25f * colors[3];
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