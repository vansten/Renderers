#include "../include/Ray.h"

#include "../include/Shape.h"
#include "../include/RaycastHit.h"

Ray::Ray()
{
}

Ray::Ray(float x0, float y0, float z0, float dx, float dy, float dz) : Origin(x0, y0, z0), Direction(dx, dy, dz)
{
	Direction.Normalize();
}

Ray::Ray(Vector3 center, Vector3 direction) : Origin(center), Direction(direction)
{
	Direction.Normalize();
}

Ray::Ray(const Ray& r) : Origin(r.Origin), Direction(r.Direction) {}


Ray::~Ray()
{
}

bool Ray::Intersects(const Shape& s, RaycastHit& hit) const
{
	hit.Clear();
	return s.Intersects(*this, hit);
}

std::string Ray::ToString() const
{
	return "[ O" + Origin.ToString() + ", D" + Direction.ToString() + " ]";
}
