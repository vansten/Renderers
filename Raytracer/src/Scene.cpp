#include "../include/Scene.h"

#include "Console.h"

#include "../include/Sphere.h"
#include "../include/Plane.h"
#include "../include/Engine.h"

#include "../include/Defines.h"

#include "../include/LightsInclude.h"

namespace raytracer
{
	Scene::Scene()
	{}

	Scene::~Scene()
	{}

	void Scene::Init()
	{
		Material* redMat = new Material(Color24::Red);
		_materials.push_back(redMat);
		Material* blueMat = new Material(Color24::Blue);
		_materials.push_back(blueMat);
		Material* magentaMat = new Material(Color24::Magenta);
		_materials.push_back(magentaMat);
		Material* greenMat = new Material(Color24::Green);
		_materials.push_back(greenMat);

		Sphere* s = new Sphere(2.5f, 0.0f, 0.0f, 1.3f, redMat);
		_shapes.push_back(s);
		s = new Sphere(-2.5f, 0.0f, -5.0f, 0.5f, greenMat);
		_shapes.push_back(s);

		Plane* p = new Plane(Vector3(0, 0, 5), Vector3(0, 0, -1), blueMat);
		_shapes.push_back(p);

		auto it = _shapes.begin();
		auto end = _shapes.end();
		for(it; it != end; ++it)
		{
			(*it)->Init();
		}

		Mesh* m = new Mesh("models/MCone.obj", Matrix::FromXYZRotationDegrees(0.0f, 0.0f, 0.0f), magentaMat);
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

		_lights.push_back(new AmbientLight(Color24::White * 0.05f));
		_lights.push_back(new DirectionalLight(Vector3(0.5f, 0.1f, 1), Color24::White));
		//_lights.push_back(new PointLight(Vector3(0, 2, -5.0f), Color24::White, 1.0f, 0.25f));
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

		auto materialsIt = _materials.begin();
		auto materialsEnd = _materials.end();
		for(materialsIt; materialsIt != materialsEnd; ++materialsIt)
		{
			delete (*materialsIt);
			(*materialsIt) = 0;
		}
		_materials.clear();

		auto lightsIt = _lights.begin();
		auto lightsEnd = _lights.end();
		for(lightsIt; lightsIt != lightsEnd; ++lightsIt)
		{
			delete (*lightsIt);
			(*lightsIt) = 0;
		}
		_lights.clear();
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
							float distSq = (hitPoints[k].Point - r.Origin).LengthSquared();
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
					Material* mat = closestShape->GetMaterial();
					if(mat != nullptr)
					{
						_image->SetPixel(i, j, mat->GetDiffuse());
					}
				}
			}
		}
	}
}