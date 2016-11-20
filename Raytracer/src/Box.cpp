#include "../include/Box.h"

namespace raytracer
{
	Box::Box(const Vector3& center, const Vector3& size, const Matrix& transform, Material* material) : _center(center), _size(size), _transform(transform), Shape(material)
	{}

	Box::Box(float cx, float cy, float cz, float sx, float sy, float sz, const Matrix& transform, Material* material) : _center(cx, cy, cz), _size(sx, sy, sz), _transform(transform), Shape(material)
	{}

	Box::Box(const Box& other) : _center(other._center), _size(other._size), _transform(other._transform), Shape(other)
	{}

	void Box::Init()
	{
		Vector3 halfSize = _size * 0.5f;
		Vector3 positions[8] = {
			_transform.MultiplyByVector3(Vector3(_center[0] - halfSize[0], _center[1] - halfSize[1], _center[2] - halfSize[2]), 1),
			_transform.MultiplyByVector3(Vector3(_center[0] - halfSize[0], _center[1] + halfSize[1], _center[2] - halfSize[2])),
			_transform.MultiplyByVector3(Vector3(_center[0] + halfSize[0], _center[1] - halfSize[1], _center[2] - halfSize[2])),
			_transform.MultiplyByVector3(Vector3(_center[0] + halfSize[0], _center[1] + halfSize[1], _center[2] - halfSize[2])),
			_transform.MultiplyByVector3(Vector3(_center[0] - halfSize[0], _center[1] - halfSize[1], _center[2] + halfSize[2])),
			_transform.MultiplyByVector3(Vector3(_center[0] - halfSize[0], _center[1] + halfSize[1], _center[2] + halfSize[2])),
			_transform.MultiplyByVector3(Vector3(_center[0] + halfSize[0], _center[1] - halfSize[1], _center[2] + halfSize[2])),
			_transform.MultiplyByVector3(Vector3(_center[0] + halfSize[0], _center[1] + halfSize[1], _center[2] + halfSize[2]))
		};
		Vector3 normals[6] = {
			_transform.MultiplyByVector3(Vector3(-1, 0, 0),0).Normalized(),
			_transform.MultiplyByVector3(Vector3(1, 0, 0),0).Normalized(),
			_transform.MultiplyByVector3(Vector3(0, -1, 0),0).Normalized(),
			_transform.MultiplyByVector3(Vector3(0, 1, 0),0).Normalized(),
			_transform.MultiplyByVector3(Vector3(0, 0, -1),0).Normalized(),
			_transform.MultiplyByVector3(Vector3(0, 0, 1),0).Normalized()
		};
		Vector2 uvs[4] = {
			Vector2(0.0f, 0.0f),
			Vector2(0.0f, 1.0f),
			Vector2(1.0f, 0.0f),
			Vector2(1.0f, 1.0f)
		};

		//Front face
		_triangles[0] = new Triangle(positions[0], positions[1], positions[2], normals[4], normals[4], normals[4], uvs[0], uvs[1], uvs[2], nullptr);
		_triangles[1] = new Triangle(positions[1], positions[3], positions[2], normals[4], normals[4], normals[4], uvs[1], uvs[3], uvs[2], nullptr);

		//Right face
		_triangles[2] = new Triangle(positions[2], positions[3], positions[6], normals[1], normals[1], normals[1], uvs[0], uvs[1], uvs[2], nullptr);
		_triangles[3] = new Triangle(positions[3], positions[7], positions[6], normals[1], normals[1], normals[1], uvs[1], uvs[3], uvs[2], nullptr);

		//Back face
		_triangles[4] = new Triangle(positions[6], positions[7], positions[4], normals[5], normals[5], normals[5], uvs[0], uvs[1], uvs[2], nullptr);
		_triangles[5] = new Triangle(positions[7], positions[5], positions[4], normals[5], normals[5], normals[5], uvs[1], uvs[3], uvs[2], nullptr);

		//Left face
		_triangles[6] = new Triangle(positions[4], positions[5], positions[0], normals[0], normals[0], normals[0], uvs[0], uvs[1], uvs[2], nullptr);
		_triangles[7] = new Triangle(positions[5], positions[1], positions[0], normals[0], normals[0], normals[0], uvs[1], uvs[3], uvs[2], nullptr);

		//Top face
		_triangles[8] = new Triangle(positions[1], positions[5], positions[3], normals[3], normals[3], normals[3], uvs[0], uvs[1], uvs[2], nullptr);
		_triangles[9] = new Triangle(positions[5], positions[7], positions[3], normals[3], normals[3], normals[3], uvs[1], uvs[3], uvs[2], nullptr);

		//Bottom face
		_triangles[10] = new Triangle(positions[4], positions[0], positions[6], normals[2], normals[2], normals[2], uvs[0], uvs[1], uvs[2], nullptr);
		_triangles[11] = new Triangle(positions[0], positions[2], positions[6], normals[2], normals[2], normals[2], uvs[1], uvs[3], uvs[2], nullptr);

		for(int i = 0; i < 12; ++i)
		{
			_triangles[i]->Init();
		}
	}

	std::string Box::ToString() const
	{
		return "C[" + _center.ToString() + "], S[" + _size.ToString() + "]";
	}

	bool Box::Intersects(const Ray& r, RaycastHit& hit) const
	{
		Triangle* closest = nullptr;
		float closestDistance = FLT_MAX;
		IntersectionPoint closestIP;
		for(int i = 0; i < 12; ++i)
		{
			if(r.Intersects(*_triangles[i], hit))
			{
				IntersectionPoint intersection = hit.GetIntersectionPoints()[0];
				float distance = (intersection.Point - r.Origin).Length();
				if(distance < closestDistance)
				{
					closestDistance = distance;
					closest = _triangles[i];
					closestIP = intersection;
				}
			}
		}

		if(closest != nullptr)
		{
			hit.Clear();
			hit.AddIntersectionPoint(closestIP);
			return true;
		}

		return false;
	}
}