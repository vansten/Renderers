#include "../include/Sphere.h"

#include "Vector.h"

Sphere::Sphere(Color24 color) : Center(), Radius(0.0f)
{
	Color = color;
}

Sphere::Sphere(Vector3 center, float radius, Color24 color) : Center(center), Radius(radius)
{
	Color = color;
}

Sphere::Sphere(float x0, float y0, float z0, float radius, Color24 color) : Center(x0, y0, z0), Radius(radius)
{
	Color = color;
}

Sphere::~Sphere() {}

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
	float a = Vector3::Dot(r.Direction, r.Direction);
	float b = 2.0f * Vector3::Dot(r.Direction, v);
	float c = Vector3::Dot(v, v) - Radius * Radius;
	float det = (b*b) - (4 * a * c);
	if(det >= 0.0f)
	{
		det = sqrt(det);
		float doubleA = 2 * a;
		float t1 = (-b - det) / doubleA;
		float t2 = (-b + det) / doubleA;

		//If t1 > 0 then we have intersaction point
		if(t1 > 0.0f)
		{
			hit.AddIntersectionPoint(r.Origin + r.Direction * t1);
		}

		//If t2 > 0 we MAY have intersection point. Just check if t2 isn't equal to t1 to make sure that intersection point isn't doubled
		if(t2 > 0.0f && fabs(t1 - t2) > 0.000001f)
		{
			hit.AddIntersectionPoint(r.Origin + r.Direction * t2);
		}

		return true;
	}

	return false;
}