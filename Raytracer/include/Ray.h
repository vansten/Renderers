#pragma once

#include "Vector.h"

namespace raytracer
{
	class Shape;
	class RaycastHit;

	class Ray
	{
	public:
		Vector3 Origin;
		Vector3 Direction;

	public:
		Ray();
		Ray(float x0, float y0, float z0, float dx, float dy, float dz);
		Ray(Vector3 origin, Vector3 direction);
		Ray(const Ray& r);
		~Ray();

		bool Intersects(const Shape& s, RaycastHit& hit) const;
		std::string ToString() const;
	};
}