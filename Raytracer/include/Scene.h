#pragma once

#include <vector>

#include "Shape.h"
#include "Mesh.h"

class Scene
{
protected:
	std::vector<Shape*> _shapes;
	std::vector<Mesh*> _meshes;

public:
	Scene();
	~Scene();

	void Init();
	void Update();
	void RenderUnlit();
	void Shutdown();
};

