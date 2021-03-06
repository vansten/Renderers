#include "../include/Sphere.h"

#include "Vector.h"
#include "../include/Defines.h"

namespace raytracer
{
	Sphere::Sphere(Material* material) : Center(), Radius(0.0f), Shape(material)
	{

	}

	Sphere::Sphere(Vector3 center, float radius, Material* material) : Center(center), Radius(radius), Shape(material)
	{
	}

	Sphere::Sphere(float x0, float y0, float z0, float radius, Material* material) : Center(x0, y0, z0), Radius(radius), Shape(material)
	{
	}

	Sphere::Sphere(const Sphere& other) : Center(other.Center), Radius(other.Radius), Shape(other)
	{

	}

	Sphere::~Sphere()
	{

	}

	void Sphere::Init()
	{

	}

	void Sphere::Shutdown()
	{
	}

	std::string Sphere::ToString() const
	{
		return "[ O" + Center.ToString() + ", r = " + std::to_string(Radius) + " ]";
	}

	bool Sphere::Intersects(const Ray& r, RaycastHit& hit) const
	{
		Vector3 v = r.Origin - Center;
		if(v.Length() < Radius)
		{
			return false;
		}
		float a = Vector3::Dot(r.Direction, r.Direction);
		float b = 2.0f * Vector3::Dot(r.Direction, v);
		float c = Vector3::Dot(v, v) - Radius * Radius;
		float det = (b*b) - (4 * a * c);
		if(det >= 0.0f)
		{
			Vector3 point;
			Vector3 normal;
			det = sqrt(det);
			float doubleA = 2 * a;
			float t1 = (-b - det) / doubleA;
			float t2 = (-b + det) / doubleA;
			bool wasHit = false;

			//If t1 > 0 then we have intersaction point
			if(t1 > 0.0f)
			{
				wasHit = true;
				point = r.Origin + r.Direction * t1;
				normal = point - Center;
				normal.Normalize();
				float u = asin(normal[0]) * InvPI + 0.5f;
				float v = asin(normal[1]) * InvPI + 0.5f;
				hit.AddIntersectionPoint(IntersectionPoint(point, normal, Vector2(u, v)));
			}

			//If t2 > 0 we MAY have intersection point. Just check if t2 isn't equal to t1 to make sure that intersection point isn't doubled
			if(t2 > 0.0f && fabs(t1 - t2) > 0.000001f)
			{
				wasHit = true;
				point = r.Origin + r.Direction * t1;
				normal = point - Center;
				normal.Normalize();
				float u = asin(normal[0]) * InvPI + 0.5f;
				float v = asin(normal[1]) * InvPI + 0.5f;
				hit.AddIntersectionPoint(IntersectionPoint(point, normal, Vector2(u, v)));
			}

			return wasHit;
		}

		return false;
	}
}