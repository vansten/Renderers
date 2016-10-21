#pragma once

#include <vector>

#include "Shape.h"
#include "Mesh.h"

#include "Image.h"

class Scene
{
protected:
	std::vector<Shape*> _shapes;
	std::vector<Mesh*> _meshes;

public:
	Scene();
	~Scene();

	void Init();
	void Shutdown();

	void Render(Image* _image) const;
};

