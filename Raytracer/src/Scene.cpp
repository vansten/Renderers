#include "../include/Scene.h"

#include "Console.h"

#include "../include/Sphere.h"
#include "../include/Plane.h"
#include "../include/Engine.h"

#include "../include/Defines.h"

namespace raytracer
{
	Scene::Scene()
	{}

	Scene::~Scene()
	{}

	void Scene::Init()
	{
		Sphere* s = new Sphere(2.0f, 0.0f, 0.0f, 1.3f, Color24::Blue);
		_shapes.push_back(s);
		s = new Sphere(-2.0f, 0.0f, 0.0f, 1.3f, Color24::Red);
		_shapes.push_back(s);
		//s = new Sphere(0.0f, 0.0f, 0.0f, 1.3f, Color24::Green);
		//_shapes.push_back(s);

		auto it = _shapes.begin();
		auto end = _shapes.end();
		for(it; it != end; ++it)
		{
			(*it)->Init();
		}

		Mesh* m = new Mesh("models/MCone.obj", Matrix::FromXYZRotationDegrees(40.0f, 0.0f, 0.0f));
		_meshes.push_back(m);

		auto meshesIt = _meshes.begin();
		auto meshesEnd = _meshes.end();
		for(meshesIt; meshesIt != meshesEnd; ++meshesIt)
		{
			if(!(*meshesIt)->Init())
			{
				Console::WriteLine("Couldn't initialize mesh");
			}
		}
	}

	void Scene::Shutdown()
	{
		auto it = _shapes.begin();
		auto end = _shapes.end();
		for(it; it != end; ++it)
		{
			(*it)->Shutdown();
			delete (*it);
		}
		_shapes.clear();

		auto meshesIt = _meshes.begin();
		auto meshesEnd = _meshes.end();
		for(meshesIt; meshesIt != meshesEnd; ++meshesIt)
		{
			(*meshesIt)->Shutdown();
			delete (*meshesIt);
		}
		_meshes.clear();
	}

	void Scene::Render(Image* _image) const
	{
		if(!_image)
		{
			return;
		}

		int width = _image->GetWidth();
		int height = _image->GetHeight();
		RaycastHit hit;
		auto shapesEnd = _shapes.end();
		auto hitPoints = hit.GetIntersectionPoints();
		float halfWidth = Engine::GetInstance()->GetWidth() * 0.5f;
		float halfHeight = Engine::GetInstance()->GetHeight() * 0.5f;
		float onePerAR = halfHeight / halfWidth;
		float pixelWidth = 1.0f / halfWidth;
		float pixelHeight = 1.0f / halfHeight;
		for(int i = 0; i < width; ++i)
		{
			for(int j = 0; j < height; ++j)
			{
#if ORTHO
				//Ray r(i / 1280.0f, j / 720.0f, -10.0f, 0.0f, 0.0f, 1.0f);
				float x = -1.0f + (i + 0.5f) * pixelWidth;
				float y = 1.0f - (j + 0.5f) * pixelHeight;
				Ray r(x, y, -10.0f, 0.0f, 0.0f, 1.0f);
				r.Origin[1] *= onePerAR;
#else
				Ray r(i, j, -10, 0, 0, 1);
#endif
				Shape* closestShape = nullptr;
				float closestSq = FLT_MAX;
				for(auto shapesIt = _shapes.begin(); shapesIt != shapesEnd; ++shapesIt)
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
					_image->SetPixel(i, j, closestShape->GetColor());
				}
			}
		}
	}
}