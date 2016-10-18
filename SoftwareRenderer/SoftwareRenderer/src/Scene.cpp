#include "../include/Scene.h"

#include "../include/DeviceContext.h"
#include "../include/Mesh.h"
#include "Input.h"
#include "Console.h"
#include "TGASerializer.h"

Scene::Scene()
{
}

Scene::~Scene()
{
}

bool Scene::Init()
{
	Mesh* m = new Mesh("models/cube.obj", "textures/cube.tga", TextureWrapMode::Wrap, TextureFiltering::Bilinear);
	//m->Scale(1.5f, 1.5f, 1.5f);
	//m->Translate(0, -1, 0);
	//m->Rotate(0.0f, 45.0f, 0.0f);
	_objects.push_back(m);
	/*Mesh* m2 = new Mesh("models/cube.obj", "textures/Ice.tga", TextureWrapMode::Wrap);
	m2->Translate(0, 1, 0);
	m2->Rotate(-45.0f, 0.0f, 0.0f);
	_objects.push_back(m2);*/

	return true;
}

void Scene::Shutdown()
{
	auto it = _objects.begin();
	auto end = _objects.end();
	for(it; it != end; ++it)
	{
		delete (*it);
		(*it) = 0;
	}
}

void Scene::Update(float deltaTime)
{
	_objects[0]->Rotate(0.0f, 15.0f * deltaTime, 0.0f);
}

void Scene::Draw(DeviceContext* deviceContext)
{
	static DirectionalLight light(Vector3(1, 0, 1), Color32::White);
	auto it = _objects.begin();
	auto end = _objects.end();
	for(it; it != end; ++it)
	{
		(*it)->Draw(deviceContext, &light);
	}
}
