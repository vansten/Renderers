#include "../include/Triangle.h"

namespace raytracer
{
	Triangle::Triangle(Vector3 v1, Vector3 v2, Vector3 v3, Material* material) : _vertex1Position(v1), _vertex2Position(v2), _vertex3Position(v3), Shape(material)
	{
	}

	Triangle::Triangle(const Triangle& other) : _vertex1Position(other._vertex1Position), _vertex2Position(other._vertex2Position), _vertex3Position(other._vertex3Position), _normal(other._normal), Shape(other)
	{
	}

	Triangle::~Triangle()
	{
	}

	void Triangle::Init()
	{
		Shape::Init();
		_normal = Vector3::Cross(_vertex2Position - _vertex1Position, _vertex3Position - _vertex1Position);
		_normal.Normalize();
		_plane = new Plane(_vertex1Position, _normal, 0);
	}

	void Triangle::Shutdown()
	{
		if(_plane)
		{
			delete _plane;
			_plane = 0;
		}
	}

	std::string Triangle::ToString() const
	{
		return "[ " + _vertex1Position.ToString() + ", " + _vertex2Position.ToString() + ", " + _vertex3Position.ToString() + ", normal: " + _normal.ToString() + " ]";
	}
	
	bool Triangle::Intersects(const Ray& r, RaycastHit& hit) const
	{
		float NdotD = Vector3::Dot(_normal, r.Direction);
		if(NdotD >= 0.0f)
		{
			//Ignore backfaces
			return false;
		}

		if(!r.Intersects(*_plane, hit))
		{
			return false;
		}

		Vector3 planeIntersectionPoint = hit.GetIntersectionPoints()[0].Point;

		float dxx3 = planeIntersectionPoint[0] - _vertex3Position[0];
		float dyy3 = planeIntersectionPoint[1] - _vertex3Position[1];
		float dx12 = _vertex1Position[0] - _vertex2Position[0];
		float dx23 = _vertex2Position[0] - _vertex3Position[0];
		float dx31 = _vertex3Position[0] - _vertex1Position[0];
		float dy12 = _vertex1Position[1] - _vertex2Position[1];
		float dy23 = _vertex2Position[1] - _vertex3Position[1];
		float dy31 = _vertex3Position[1] - _vertex1Position[1];

		float a = dx12 * (planeIntersectionPoint[1] - _vertex1Position[1]) - dy12 * (planeIntersectionPoint[0] - _vertex1Position[0]);
		float b = dx23 * (planeIntersectionPoint[1] - _vertex2Position[1]) - dy23 * (planeIntersectionPoint[0] - _vertex2Position[0]);
		float c = dx31 * dyy3 - dy31 * dxx3;
		if(
			a >= 0.0f &&
			b >= 0.0f &&
			c >= 0.0f
			)
		{
			return true;
		}

		return false;
	}
}