#include "..\include\Block.h"
#include "../include/Engine.h"

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

void Block::Render(Scene* _scene)
{
	auto shapes = _scene->GetShapes();
	auto shapesEnd = shapes.end();
	RaycastHit hit;
	auto hitPoints = hit.GetIntersectionPoints();
	float halfWidth = Engine::GetInstance()->GetWidth() * 0.5f;
	float halfHeight = Engine::GetInstance()->GetHeight() * 0.5f;
	float onePerAR = halfHeight / halfWidth;
	int maxI = _x0 + _width;
	int maxY = _y0 + _height;
	for(int i = _x0; i < maxI; ++i)
	{
		for(int j = _y0; j < maxY; ++j)
		{
#if ORTHO
			//Ray r(i / 1280.0f, j / 720.0f, -10.0f, 0.0f, 0.0f, 1.0f);
			Ray r(i / halfWidth - 1.0f, j / halfHeight - 1.0f, -10.0f, 0.0f, 0.0f, 1.0f);
			r.Origin[1] *= onePerAR;
#else
			Ray r(i, j, -10, 0, 0, 1);
#endif
			Shape* closestShape = nullptr;
			float closestSq = FLT_MAX;
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