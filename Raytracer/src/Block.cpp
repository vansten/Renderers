#include "..\include\Block.h"
#include "../include/Engine.h"

#include "../include/Defines.h"

#include "Console.h"

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

	void Block::Render(Scene* scene)
	{
		if(!scene)
		{
			return;
		}

		auto shapes = scene->GetShapes();
		auto shapesEnd = shapes.end();
		float halfWidth = Engine::GetInstance()->GetWidth() * 0.5f;
		float halfHeight = Engine::GetInstance()->GetHeight() * 0.5f;
		float onePerAR = halfHeight / halfWidth;
		int maxI = _x0 + _width;
		int maxY = _y0 + _height;
		float pixelWidth = 1.0f / halfWidth;
		float pixelHeight = 1.0f / halfHeight;
		RaycastHit hit;
		auto hitPoints = hit.GetIntersectionPoints();
		Shape* closestShape = nullptr;
		float closestSq = FLT_MAX;
		Ray r;
		Vector3 origin;
		Vector3 direction = Vector3(0, 0, 1);

		for(int i = _x0; i < maxI; ++i)
		{
			for(int j = _y0; j < maxY; ++j)
			{
#if ORTHO
				origin[0] = -1.0f + (i + 0.5f) * pixelWidth;
				origin[1] = 1.0f - (j + 0.5f) * pixelHeight;
				origin[2] = -10.0f;
#else
				origin[0] = (i / halfWidth) - 1.0f;
				origin[1] = (j / halfHeight) - 1.0f;
				origin[2] = -10.0f;
#endif
				r.Origin = origin;
				r.Direction = direction;
				r.Origin[1] *= onePerAR;
				closestShape = nullptr;
				closestSq = FLT_MAX;
				for(auto shapesIt = shapes.begin(); shapesIt != shapesEnd; ++shapesIt)
				{
					if(r.Intersects(*(*shapesIt), hit))
					{
						hitPoints = hit.GetIntersectionPoints();
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

				if(closestShape != nullptr)
				{
					_renderedImage->SetPixel(i - _x0, j - _y0, closestShape->GetColor());
				}
			}
		}
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