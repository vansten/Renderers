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
		Matrix _transform;

	public:
		Mesh(const char* filename, const Matrix& transform, Material* material);
		~Mesh();

		bool Init();
		void Shutdown();

		inline std::vector<Triangle*> GetTriangles()
		{
			return _triangles;
		}

		inline Vector3 GetTransformedVector(Vector3 vertex, float w)
		{
			return _transform.MultiplyByVector3(vertex, w);
		}
	};
}