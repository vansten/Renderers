#include "../include/Plane.h"

Plane::Plane() : _planePoint(0, 0, 0), _normal(0, 1, 0), _d(0) {}

Plane::Plane(float x0, float y0, float z0, float nx, float ny, float nz) : _planePoint(x0, y0, z0), _normal(nx, ny, nz), _d(0) {}

Plane::Plane(Vector3 point, Vector3 normal) : _planePoint(point), _normal(normal), _d(0) {}

Plane::~Plane() {}

void Plane::Init()
{
	//n.x * p.x + n.y * p.y + n.z * p.z + D = 0
	//D = -Dot(n, p)
	//d = -D = Dot(n, p)
	_normal.Normalize();
	_d = Vector3::Dot(_planePoint, _normal);
}

void Plane::Shutdown()
{
	_d = 0.0f;
}

std::string Plane::ToString() const
{
	return "[ P" + _planePoint.ToString() + ", N" + _normal.ToString() + " , d=" + std::to_string(_d) + " ]";
}

const Vector3& Plane::GetNormal() const
{
	return _normal;
}

const Vector3& Plane::GetPoint() const
{
	return _planePoint;
}

bool Plane::Intersects(const Ray& r, RaycastHit& hit) const
{
	float NdotD = Vector3::Dot(_normal, r.Direction);
	if(NdotD == 0.0f)
	{
		//Ray's direction is perpendicular to plane's normal so there is no intersection for sure
		return false;
	}

	//t = (d - NdotO)/(NdotD)
	float NdotO = Vector3::Dot(r.Origin, _normal);
	float t = (_d - NdotO) / (NdotD);
	if(t < 0.0f)
	{
		return false;
	}

	hit.AddIntersectionPoint(r.Origin + r.Direction * t);

	return true;
}