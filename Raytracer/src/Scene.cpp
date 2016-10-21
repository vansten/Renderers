#include "../include/Scene.h"

#include "Console.h"

#include "../include/Sphere.h"
#include "../include/Plane.h"
#include "../include/Engine.h"

Scene::Scene()
{
}

Scene::~Scene()
{
}

void Scene::Init()
{
	Sphere* s = new Sphere(1.0f, 0.0f, 0, 0.1f, Color24::Blue);
	_shapes.push_back(s);
	s = new Sphere(-1.0f, 0.0f, 0.0f, 0.1f, Color24::Red);
	_shapes.push_back(s);
	//Plane* p = new Plane(Vector3(0, 0, 0), Vector3(0.0f, 1.0f, 1.0f));
	//_shapes.push_back(p);

	auto it = _shapes.begin();
	auto end = _shapes.end();
	for(it; it != end; ++it)
	{
		(*it)->Init();
	}

	/*Vector3 r1Center(0, 0, -20);
	Vector3 r3Center(0, 10, 10);
	Vector3 rayToShape = s->Center - r1Center;
	Ray r1(r1Center, rayToShape);
	Ray r2(r1Center, Vector3(0, 1, 0));
	Ray r3(r3Center, Vector3(0, 0, -1));
	
	RaycastHit hit;
	if(r1.Intersects(*s, hit))
	{
		const std::vector<Vector3>& intersectionPoints = hit.GetIntersectionPoints();
		int intersectionsCount = (int)intersectionPoints.size();
		Console::WriteFormat("There was an intersection between\nr1 (%s)\nand sphere (%s).\nThe intersection has %i point%s:\n", r1.ToString().c_str(), s->ToString().c_str(), intersectionsCount, (intersectionsCount > 1 ? "s" : ""));
		for(int i = 0; i < intersectionsCount; ++i)
		{
			Console::WriteFormat("%s%s", intersectionPoints[i].ToString().c_str(), (i == intersectionsCount - 1 ? "" : ", "));
		}
		Console::Write("\n\n");
	}
	if(r2.Intersects(*s, hit))
	{
		const std::vector<Vector3>& intersectionPoints = hit.GetIntersectionPoints();
		int intersectionsCount = (int)intersectionPoints.size();
		Console::WriteFormat("There was an intersection between\nr2 (%s)\nand sphere (%s).\nThe intersection has %i point%s:\n", r2.ToString().c_str(), s->ToString().c_str(), intersectionsCount, (intersectionsCount > 1 ? "s" : ""));
		for(int i = 0; i < intersectionsCount; ++i)
		{
			Console::WriteFormat("%s%s", intersectionPoints[i].ToString().c_str(), (i == intersectionsCount - 1 ? "" : ", "));
		}
		Console::Write("\n\n");
	}
	if(r3.Intersects(*s, hit))
	{
		const std::vector<Vector3>& intersectionPoints = hit.GetIntersectionPoints();
		int intersectionsCount = (int)intersectionPoints.size();
		Console::WriteFormat("There was an intersection between\nr3 (%s)\nand sphere (%s).\nThe intersection has %i point%s:\n", r3.ToString().c_str(), s->ToString().c_str(), intersectionsCount, (intersectionsCount > 1 ? "s" : ""));
		for(int i = 0; i < intersectionsCount; ++i)
		{
			Console::WriteFormat("%s%s", intersectionPoints[i].ToString().c_str(), (i == intersectionsCount - 1 ? "" : ", "));
		}
		Console::Write("\n\n");
	}

	if(r2.Intersects(*p, hit))
	{
		const std::vector<Vector3>& intersectionPoints = hit.GetIntersectionPoints();
		int intersectionsCount = (int)intersectionPoints.size();
		Console::WriteFormat("There was an intersection between\nr2 (%s)\nand plane (%s).\nThe intersection has %i point%s:\n", r2.ToString().c_str(), p->ToString().c_str(), intersectionsCount, (intersectionsCount > 1 ? "s" : ""));
		for(int i = 0; i < intersectionsCount; ++i)
		{
			Console::WriteFormat("%s%s", intersectionPoints[i].ToString().c_str(), (i == intersectionsCount - 1 ? "" : ", "));
		}
		Console::Write("\n\n");
	}*/

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
	for(int i = 0; i < width; ++i)
	{
		for(int j = 0; j < height; ++j)
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
