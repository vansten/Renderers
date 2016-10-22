#pragma once

#include "Shape.h"

namespace raytracer
{
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
}