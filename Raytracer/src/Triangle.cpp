#include "../include/Triangle.h"
#include "../include/Mesh.h"

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

		Vector3 v1v2 = _vertex2Position - _vertex1Position;
		Vector3 v1v3 = _vertex3Position - _vertex1Position;
		Vector3 pVec = Vector3::Cross(r.Direction, v1v3);
		float det = Vector3::Dot(v1v2, pVec);

		if(fabs(det) < 0.000001f) return false;

		float invDet = 1.0f / det;

		Vector3 tVec = r.Origin - _vertex1Position;
		float u = Vector3::Dot(tVec, pVec) * invDet;
		if(u < 0 || u > 1) return false;

		Vector3 qVec = Vector3::Cross(tVec, v1v2);
		float v = Vector3::Dot(r.Direction, qVec) * invDet;
		if(v < 0 || (u + v) > 1) return false;

		return true;
	}
}