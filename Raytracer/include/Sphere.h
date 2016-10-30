#pragma once
#include "Shape.h"

namespace raytracer
{
	class Sphere : public Shape
	{
	public:
		Vector3 Center;
		float Radius;

	public:
		Sphere(Material* material);
		Sphere(Vector3 center, float radius, Material* material);
		Sphere(float x0, float y0, float z0, float radius, Material* material);
		Sphere(const Sphere& other);
		virtual ~Sphere();

		virtual void Init() override;
		virtual void Shutdown() override;
		virtual std::string ToString() const override;

	protected:
		virtual bool Intersects(const Ray& r, RaycastHit& hit) const override;
	};
}