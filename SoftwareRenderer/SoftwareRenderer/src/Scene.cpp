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
	std::string modelName = "models/cube.obj";
	std::string sphereName = "models/sphere.obj";
	std::string cubeTexture = "textures/cube.tga";
	std::string iceTexture = "textures/Ice.tga";
#if _DEBUG
	Mesh* m = new Mesh(modelName, cubeTexture);
	m->Scale(2.0f, 2.0f, 2.0f);
	_objects.push_back(m);
#else
	Mesh* m = new Mesh(modelName, cubeTexture, TextureWrapMode::Wrap, TextureFiltering::Bilinear);
	m->AddToUVs(0.0f, 0.3f);
	m->Scale(0.5f, 0.5f, 0.5f);
	m->Translate(-1.0f, 1.0f, 0.0f);
	_objects.push_back(m);
	m = new Mesh(modelName, cubeTexture, TextureWrapMode::Clamp, TextureFiltering::Nearest);
	m->AddToUVs(0.0f, 0.3f);
	m->Translate(-1.0f, 0.9f, 1.0f);
	m->Scale(0.5f, 0.5f, 0.5f);
	_objects.push_back(m);
	m = new Mesh(modelName, cubeTexture, TextureWrapMode::Clamp, TextureFiltering::Bilinear);
	m->AddToUVs(0.5f, 0.0f);
	m->Translate(1.0f, 0.5f, 3.0f);
	_objects.push_back(m);
	m = new Mesh(modelName, cubeTexture, TextureWrapMode::Wrap, TextureFiltering::Bilinear);
	m->AddToUVs(0.5f, 0.0f);
	m->Translate(1.0f, 0.7f, 2.5f);
	_objects.push_back(m);
	m = new Mesh(sphereName, iceTexture);
	m->Translate(-1.0f, -1.0f, -2.0f);
	m->Scale(0.5f, 0.5f, 0.5f);
	_objects.push_back(m);
	m = new Mesh(sphereName, cubeTexture);
	m->Translate(1.0f, -1.0f, -2.0f);
	_objects.push_back(m);
	m = new Mesh(modelName, iceTexture);
	m->Translate(-0.4f, -0.6f, -1.0f);
	m->Rotate(0.0f, 45.0f, 0.0f);
	_objects.push_back(m);
#endif

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

float timer = 0.0f;

void Scene::Update(float deltaTime)
{
	timer += deltaTime;
#if _DEBUG
#else
	_objects[0]->Rotate(0.0f, 15.0f * deltaTime, 0.0f);
	float sinValue = sin(timer) - sin(timer - deltaTime);
	_objects[1]->Translate(0.0f, 0.0f, sinValue * 3.0f);
	_objects[3]->Translate(sinValue * 2.0f, 0.0f, 0.0f);
	_objects[5]->Translate(0.0f, sinValue * 1.5f, 0.0f);
#endif
}

void Scene::Draw(DeviceContext* deviceContext)
{
#if _DEBUG
	static DirectionalLight light(Vector3(0, 0, 1), Color32::White * 0.2f);
	static SpotLight spotlight(Vector3(0, 0, -3), Vector3(0, 0, 1), Color32::Red, Vector2(1.f, 0.5f), 20.f, 6.0f);
#else
	static DirectionalLight light(Vector3(-1, 0, 0), Color32::Red);
	static SpotLight spotlight(Vector3(0, -1, -5), Vector3(0, 0, 1), Color32::Green, Vector2(0.001f, 0.001f), 20.0f, 3.0f);
#endif
	auto it = _objects.begin();
	auto end = _objects.end();
	for(it; it != end; ++it)
	{
		(*it)->Draw(deviceContext, &light, &spotlight);
	}
}
