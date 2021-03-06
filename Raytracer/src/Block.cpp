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
		auto lights = scene->GetLights();
		auto lightsBegin = lights.begin();
		auto lightsEnd = lights.end();
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
				pixel.Z = 0.0f;
#if ANTI_ALIASING
					_renderedImage->SetPixel(i - _x0, j - _y0, CastRays(pixel, MAX_STEPS, pixelWidth, pixelHeight, shapesBegin, shapesEnd, camera, lightsBegin, lightsEnd));
#else
					camera->ConstructRay(r, pixel);
					_renderedImage->SetPixel(i - _x0, j - _y0, CastRay(r, shapesBegin, shapesEnd, lightsBegin, lightsEnd, MAX_RECURSIVE));
#endif
			}
		}
	}

	Color24 Block::CastRay(const Ray& r, std::vector<Shape*>::iterator shapesBegin, std::vector<Shape*>::iterator shapesEnd, std::vector<Light*>::iterator lightsBegin, std::vector<Light*>::iterator lightsEnd, int steps)
	{
		RaycastHit hit;
		Shape* closestShape = nullptr;
		float closestSq = FLT_MAX;
		IntersectionPoint closestIP;
		for(auto shapesIt = shapesBegin; shapesIt != shapesEnd; ++shapesIt)
		{
			if(r.Intersects(*(*shapesIt), hit))
			{
				auto hitPoints = hit.GetIntersectionPoints();
				int hitPointsSize = hitPoints.size();
				for(int k = 0; k < hitPointsSize; ++k)
				{
					float distSq = (hitPoints[k].Point - r.Origin).LengthSquared();
					if(distSq < closestSq)
					{
						closestSq = distSq;
						closestShape = (*shapesIt);
						closestIP = hitPoints[k];
					}
				}
			}
		}

		if(closestShape)
		{
			float reflectance = closestShape->GetMaterial()->GetReflectance();
			float refraction = closestShape->GetMaterial()->GetRefraction();
			float diffuse = clamp(1.0f - reflectance - refraction);

			Color24 reflectionColor;
			Color24 refractionColor;

			if(steps > 0)
			{

				if(closestShape->GetMaterial()->IsReflective())
				{
					Ray ray(closestIP.Point - r.Direction * 0.0001f, Reflect(r.Direction, closestIP.Normal));
					reflectionColor = CastRay(ray, shapesBegin, shapesEnd, lightsBegin, lightsEnd, steps - 1);
				}

				if(closestShape->GetMaterial()->IsRefractive())
				{
					float cos1 = Vector3::Dot(r.Direction, closestIP.Normal);
					float cos2 = sqrt(1 - reflectance * reflectance * (1 - cos1 * cos1));
					Vector3 newDirection = (r.Direction * reflectance) + closestIP.Normal * (reflectance * cos1 - cos2);
					Ray ray(closestIP.Point + r.Direction * 0.0001f, newDirection);
					refractionColor = CastRay(ray, shapesBegin, shapesEnd, lightsBegin, lightsEnd, steps - 1);
				}
			}

			Color24 diffuseColor = Color24::Black;
#if LIGHTS
			if(closestShape->CalculateLights())
			{
				RaycastHit secondHit;
				Ray ray;
				ray.Origin = closestIP.Point;
				for(auto lightsIt = lightsBegin; lightsIt != lightsEnd; ++lightsIt)
				{
					diffuseColor += (*lightsIt)->Affect(closestShape, closestIP, Engine::GetInstance()->GetCamera(), shapesBegin, shapesEnd);
				}
			}
			else
			{
				diffuseColor = closestShape->GetMaterial()->GetDiffuse(closestIP.UVs);
			}
#else
			diffuseColor = closestShape->GetMaterial()->GetDiffuse(closestIP.UVs);
#endif
			return reflectance * reflectionColor + refractionColor + diffuse * diffuseColor;
			return closestShape->GetMaterial()->GetDiffuse(closestIP.UVs);
		}

		return _backgroundColor;
	}

	Color24 Block::CastRays(const Pixel& center, int maxSteps, float pixelWidth, float pixelHeight, std::vector<Shape*>::iterator shapesBegin, std::vector<Shape*>::iterator shapesEnd, const Camera* camera, std::vector<Light*>::iterator lightsBegin, std::vector<Light*>::iterator lightsEnd)
	{
		if(maxSteps == 0)
		{
			Ray r;
			camera->ConstructRay(r, center);
			return CastRay(r, shapesBegin, shapesEnd, lightsBegin, lightsEnd, MAX_RECURSIVE);
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
			colors[i] = CastRay(rays[i], shapesBegin, shapesEnd, lightsBegin, lightsEnd, MAX_RECURSIVE);
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
				colors[i] = CastRays(subCenters[i], maxSteps - 1, halfPixelW, halfPixelH, shapesBegin, shapesEnd, camera, lightsBegin, lightsEnd);
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