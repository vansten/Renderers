#include "../include/Scene.h"

#include "Console.h"

#include "../include/Sphere.h"
#include "../include/Plane.h"

Scene::Scene()
{
}

Scene::~Scene()
{
}

void Scene::Init()
{
	Sphere* s = new Sphere(0, 0, 0, 10);
	_shapes.push_back(s);
	Plane* p = new Plane(Vector3(0, 0, 0), Vector3(0.0f, 1.0f, 1.0f));
	_shapes.push_back(p);

	auto it = _shapes.begin();
	auto end = _shapes.end();
	for(it; it != end; ++it)
	{
		(*it)->Init();
	}

	Vector3 r1Center(0, 0, -20);
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
	}

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

void Scene::Update()
{
}

void Scene::RenderUnlit()
{

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