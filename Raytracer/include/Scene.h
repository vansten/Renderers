#pragma once

#include <vector>

#include "Shape.h"
#include "Mesh.h"

#include "Image.h"

#define ORTHO 1

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

	inline const std::vector<Shape*> GetShapes()
	{
		return _shapes;
	}

	inline const std::vector<Mesh*> GetMeshes()
	{
		return _meshes;
	}
};

