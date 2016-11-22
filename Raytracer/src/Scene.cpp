#include "../include/Scene.h"

#include "Console.h"

#include "../include/Engine.h"

#include "../include/Defines.h"

#include "../include/ShapesInclude.h"
#include "../include/LightsInclude.h"

namespace raytracer
{
	Scene::Scene()
	{}

	Scene::~Scene()
	{}

	void Scene::Init()
	{
		Material* redMat = new Material(Color24::Red, Color24::White * 0.01f, 64.0f);
		_materials.push_back(redMat);
		Material* blueMat = new Material(Color24::Blue, Color24::White * 0.01f, 128.0f);
		_materials.push_back(blueMat);
		Material* magentaMat = new Material(Color24::Magenta, Color24::White, 8.0f);
		_materials.push_back(magentaMat);
		Material* greenMat = new Material(Color24::Green, Color24::White, 256.0f);
		_materials.push_back(greenMat);
		Material* grayMat = new Material(Color24::White * 0.4f, Color24::White * 0.01f, 256.0f);
		_materials.push_back(grayMat);
		Material* whiteMat = new Material(Color24::White, Color24::White, 256.0f);
		_materials.push_back(whiteMat);
		Material* blackMat = new Material(Color24::Black, Color24::Black, 0.0f);
		_materials.push_back(blackMat);
		Material* textureMat = new MaterialTexture("textures/sphere.tga", Color24::White * 0.2f, 32.0f);
		_materials.push_back(textureMat);
		Material* iceTextureMat = new MaterialTexture("textures/ice.tga", Color24::White, 64.0f);
		_materials.push_back(iceTextureMat);
		Material* refractionMat = new Material(Color24::White, Color24::White, 128.0f, 0.0f, 1.0f);
		_materials.push_back(refractionMat);
		Material* reflectionMat = new Material(Color24::White, Color24::White, 128.0f, 1.0f, 0.0f);
		_materials.push_back(reflectionMat);
		
		float size = 5.0f;
		Box* b = new Box(0, 0, 0.0f, size, size, size, Matrix::Identity, textureMat);
		_shapes.push_back(b);
		b = new Box(size, 0, -size, size, size, size, Matrix::Identity, blueMat);
		_shapes.push_back(b);
		b = new Box(-size , 0, -size, size, size, size, Matrix::Identity, redMat);
		_shapes.push_back(b);
		b = new Box(0, -size, -size, size, size, size, Matrix::Identity, grayMat);
		_shapes.push_back(b);
		b = new Box(0, size * 0.5f, -size, size * 0.3f, size * 0.000001f, size * 0.3f, Matrix::Identity, whiteMat);
		_shapes.push_back(b);

		float radius = size * 0.15f;
		Sphere* reflectiveSphere = new Sphere(-size * 0.3f + radius, -size * 0.5f + radius, -size * 0.8f, radius, reflectionMat);
		_shapes.push_back(reflectiveSphere);
		Sphere* refractiveSphere = new Sphere(size * 0.3f - radius, -size * 0.5f + radius, -size * 1.1f, radius, refractionMat);
		_shapes.push_back(refractiveSphere);
		
		auto it = _shapes.begin();
		auto end = _shapes.end();
		for(it; it != end; ++it)
		{
			(*it)->Init();
		}

		_lights.push_back(new AmbientLight(Color24::White * 0.05f));
		//_lights.push_back(new DirectionalLight(Vector3(0, 0, 1), Color24::White * 0.2f));
		_lights.push_back(new PointLight(Vector3(0.0f, size * 0.15f, -size), Color24::White, 1.0f, 0.35f));
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
}