#pragma once

#include "Triangle.h"
#include "Matrix.h"

namespace raytracer
{
	class Mesh
	{
	protected:
		std::vector<Triangle*> _triangles;
		Material* _material;

	public:
		Mesh(const char* filename, const Matrix& transform, Material* material);
		~Mesh();

		bool Init();
		void Shutdown();

		inline std::vector<Triangle*> GetTriangles()
		{
			return _triangles;
		}
	};
}