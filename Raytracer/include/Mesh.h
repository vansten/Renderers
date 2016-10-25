#pragma once

#include "Triangle.h"
#include "Matrix.h"

namespace raytracer
{
	class Mesh
	{
	protected:
		std::vector<Triangle*> _triangles;

	public:
		Mesh(const char* filename, const Matrix& transform, Color24 color = Color24::Magenta);
		~Mesh();

		bool Init();
		void Shutdown();

		inline std::vector<Triangle*> GetTriangles()
		{
			return _triangles;
		}
	};
}