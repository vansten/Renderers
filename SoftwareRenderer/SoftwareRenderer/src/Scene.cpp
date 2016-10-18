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
	Mesh* m = new Mesh("models/cube.obj", "textures/cube.tga", TextureWrapMode::Clamp);
	//m->Scale(0.5f, 0.5f, 0.5f);
	m->Translate(0, -1, 0);
	m->Rotate(0.0f, 45.0f, 0.0f);
	_objects.push_back(m);

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

float time = 0.0f;
int frames = 0;

void Scene::Update(float deltaTime)
{
	_objects[0]->Rotate(0.0f, 15.0f * deltaTime, 0.0f);
}

void Scene::Draw(DeviceContext* deviceContext)
{
	static DirectionalLight light(Vector3(1, 0, 1), Color32::Red);
	auto it = _objects.begin();
	auto end = _objects.end();
	for(it; it != end; ++it)
	{
		(*it)->Draw(deviceContext, &light);
	}
}
