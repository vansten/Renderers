#pragma once

#include "Shape.h"

class Mesh
{
protected:
	Shape* _boundingShape;

public:
	Mesh();
	~Mesh();

	bool Init();
	void Shutdown();
};

