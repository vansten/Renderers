#pragma once

#include "../include/Matrix.h"
#include "Triangle.h"

namespace raytracer
{
	class Box : public Shape
	{
	protected:
		Triangle* _triangles[12];
		Matrix _transform;
		Vector3 _center;
		Vector3 _size;

	public:
		Box(const Vector3& center, const Vector3& size, const Matrix& transform, Material* material = nullptr);
		Box(float cx, float cy, float cz, float sx, float sy, float sz, const Matrix& transform, Material* material = nullptr);
		Box(const Box& other);

		virtual void Init() override;
		virtual std::string ToString() const override;

	protected:
		virtual bool Intersects(const Ray& r, RaycastHit& hit) const override;
	};
}