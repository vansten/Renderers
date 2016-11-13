#include "../include/Plane.h"

namespace raytracer
{
	Plane::Plane(float density, Material* material) : _planePoint(0, 0, 0), _normal(0, 1, 0), _d(0), _density(density), Shape(material)
	{}

	Plane::Plane(float x0, float y0, float z0, float nx, float ny, float nz, float density, Material* material) : _planePoint(x0, y0, z0), _normal(nx, ny, nz), _d(0), _density(density), Shape(material)
	{
	}

	Plane::Plane(Vector3 point, Vector3 normal, float density, Material* material) : _planePoint(point), _normal(normal), _d(0), _density(density), Shape(material)
	{

	}

	Plane::Plane(const Plane& other) : _planePoint(other._planePoint), _normal(other._normal), _d(other._d), _density(other._density), Shape(other)
	{

	}

	Plane::~Plane()
	{

	}

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
		if(NdotD >= 0.0f)
		{
			//Ray's direction is perpendicular to plane's normal so there is no intersection for sure
			return false;
		}

		//t = (d - NdotO)/(NdotD)
		//float NdotO = Vector3::Dot(r.Origin, _normal);
		//float t = (_d - NdotO) / (NdotD);

		float NdotO = Vector3::Dot(r.Origin - _planePoint, _normal);
		float t = -(NdotO) / (NdotD);

		if(t <= 0.0f)
		{
			return false;
		}

		Vector3 point = r.Origin + r.Direction * t;
		Vector3 oneMinusN = Vector3::One - _normal;
		Vector3 a;
		Vector3 b;
		if(oneMinusN[0] != 0.0f)
		{
			a = Vector3(oneMinusN[0], 0.0f, 0.0f);
			if(oneMinusN[1] != 0.0f)
			{
				b = Vector3(0.0f, oneMinusN[1], 0.0f);
			}
			else
			{
				b = Vector3(0.0f, 0.0f, oneMinusN[2]);
			}
		}
		else
		{
			a = Vector3(0.0f, oneMinusN[1], 0.0f);
			b = Vector3(0.0f, 0.0f, oneMinusN[2]);
		}

		Vector2 uv = Vector2(
			Vector3::Dot(a, (point - _planePoint) * _density),
			Vector3::Dot(b, (point - _planePoint) * _density)
		);

		hit.AddIntersectionPoint(IntersectionPoint(point, _normal, uv));

		return true;
	}
}