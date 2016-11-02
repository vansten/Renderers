#pragma once

#include "Plane.h"

namespace raytracer
{
	class Triangle : public Shape
	{
	protected:
		Plane* _plane;
		Vector3 _vertex1Position;
		Vector3 _vertex2Position;
		Vector3 _vertex3Position;
		Vector3 _normal;

	public:
		Triangle(Vector3 v1, Vector3 v2, Vector3 v3, Material* material);
		Triangle(const Triangle& other);
		virtual ~Triangle();
		virtual void Init() override;
		virtual void Shutdown() override;
		virtual std::string ToString() const override;
		
	protected:
		virtual bool Intersects(const Ray& r, RaycastHit& hit) const override;
	};
}