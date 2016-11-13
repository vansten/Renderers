#include "../include/Triangle.h"
#include "../include/Mesh.h"

namespace raytracer
{
	Triangle::Triangle(Vector3 v1, Vector3 v2, Vector3 v3, Vector3 vn1, Vector3 vn2, Vector3 vn3, Vector2 vt1, Vector2 vt2, Vector2 vt3, Material* material) : _vertex1(v1, vn1, vt1), _vertex2(v2, vn2, vt2), _vertex3(v3, vn3, vt3), Shape(material)
	{
	}

	Triangle::Triangle(const Triangle& other) : _vertex1(other._vertex1), _vertex2(other._vertex2), _vertex3(other._vertex3), _normal(other._normal), Shape(other)
	{
	}

	Triangle::~Triangle()
	{
	}

	void Triangle::Init()
	{
		Shape::Init();
		_normal = Vector3::Cross(_vertex2.Position - _vertex1.Position, _vertex3.Position - _vertex1.Position);
		_normal.Normalize();
		_plane = new Plane(_vertex1.Position, _normal, 1.0f, 0);
		_vertex1.Normal.Normalize();
		_vertex2.Normal.Normalize();
		_vertex3.Normal.Normalize();
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
		return "[ " + _vertex1.Position.ToString() + ", " + _vertex2.Position.ToString() + ", " + _vertex3.Position.ToString() + ", normal: " + _normal.ToString() + " ]";
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

		Vector3 v1v2 = _vertex2.Position - _vertex1.Position;
		Vector3 v1v3 = _vertex3.Position - _vertex1.Position;
		Vector3 pVec = Vector3::Cross(r.Direction, v1v3);
		float det = Vector3::Dot(v1v2, pVec);

		if(fabs(det) < 0.000001f) return false;

		float invDet = 1.0f / det;

		Vector3 tVec = r.Origin - _vertex1.Position;
		float u = Vector3::Dot(tVec, pVec) * invDet;
		if(u < 0 || u > 1) return false;

		Vector3 qVec = Vector3::Cross(tVec, v1v2);
		float v = Vector3::Dot(r.Direction, qVec) * invDet;
		if(v < 0 || (u + v) > 1) return false;

		Vector3 planePoint = hit.GetIntersectionPoints()[0].Point;
		Vector3 normal = _vertex1.Normal * (1 - u - v) + _vertex2.Normal * u + _vertex3.Normal * v;
		Vector2 uv = _vertex1.UV * (1 - u - v) + _vertex2.UV * u + _vertex3.UV * v;
		hit.Clear();
		hit.AddIntersectionPoint(IntersectionPoint(planePoint, normal, uv));

		return true;
	}
}