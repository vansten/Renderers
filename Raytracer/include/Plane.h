#pragma once
#include "Shape.h"
#include "Vector.h"

namespace raytracer
{
	class Plane : public Shape
	{
	protected:
		//Ax + By + Cz + D = 0
		Vector3 _planePoint; //x,y,z
		Vector3 _normal; //A, B, C
		float _d; //-D

	public:

		Plane(Material* material);
		Plane(float x0, float y0, float z0, float nx, float ny, float nz, Material* material);
		Plane(Vector3 point, Vector3 normal, Material* material);
		Plane(const Plane& other);
		virtual ~Plane();

		virtual void Init() override;
		virtual void Shutdown() override;
		virtual std::string ToString() const override;

		const Vector3& GetNormal() const;
		const Vector3& GetPoint() const;

	protected:
		virtual bool Intersects(const Ray& r, RaycastHit& hit) const override;
	};
}