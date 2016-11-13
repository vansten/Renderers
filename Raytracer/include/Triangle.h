#pragma once

#include "../include/Vertex.h"
#include "Plane.h"

namespace raytracer
{
	class Triangle : public Shape
	{
	protected:
		Plane* _plane;
		VertexPositionUVNormal _vertex1;
		VertexPositionUVNormal _vertex2;
		VertexPositionUVNormal _vertex3;
		Vector3 _normal;

	public:
		Triangle(Vector3 v1, Vector3 v2, Vector3 v3, Vector3 vn1, Vector3 vn2, Vector3 vn3, Vector2 vt1, Vector2 vt2, Vector2 vt3, Material* material);
		Triangle(const Triangle& other);
		virtual ~Triangle();
		virtual void Init() override;
		virtual void Shutdown() override;
		virtual std::string ToString() const override;
		
	protected:
		virtual bool Intersects(const Ray& r, RaycastHit& hit) const override;
	};
}