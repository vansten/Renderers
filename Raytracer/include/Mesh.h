#pragma once

#include "Triangle.h"
#include "Matrix.h"
#include "ShapesInclude.h"

namespace raytracer
{
	class Mesh : public Shape
	{
	protected:
		std::vector<Triangle*> _triangles;
		Matrix _transform;
		Box* _boundingBox;

	public:
		Mesh(const char* filename, const Matrix& transform, Material* material);
		Mesh(const Mesh& other);
		~Mesh();

		virtual void Init() override;
		virtual void Shutdown() override;

	protected:
		virtual bool Intersects(const Ray& r, RaycastHit& hit) const override;
	};
}