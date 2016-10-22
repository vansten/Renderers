#include "../include/Mesh.h"

namespace raytracer
{
	Mesh::Mesh()
	{
		_boundingShape = 0;
	}


	Mesh::~Mesh()
	{}

	bool Mesh::Init()
	{
		return true;
	}

	void Mesh::Shutdown()
	{
		if(_boundingShape)
		{
			_boundingShape->Shutdown();
			delete _boundingShape;
			_boundingShape = 0;
		}
	}
}