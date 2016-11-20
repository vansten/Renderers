#include "../include/Mesh.h"

#include "OBJLoader.h"

namespace raytracer
{
	Mesh::Mesh(const char* filename, const Matrix& transform, Material* material) : _transform(transform), Shape(material)
	{
		std::vector<Vector3> positions;
		std::vector<Vector3> normals;
		std::vector<Vector2> texCoords;
		std::vector<Face> faces;

		OBJLoader::LoadOBJ(filename, positions, texCoords, normals, faces);

		int size = (int)faces.size();
		for(int i = 0; i < size; ++i)
		{
			Face f = faces[i];
			Vector3 transformedPos0 = transform.MultiplyByVector3(positions[f.VertexPositions[0]], 1);
			Vector3 transformedPos1 = transform.MultiplyByVector3(positions[f.VertexPositions[1]], 1);
			Vector3 transformedPos2 = transform.MultiplyByVector3(positions[f.VertexPositions[2]], 1);
			Vector3 transformedNormal0 = transform.MultiplyByVector3(normals[f.VertexNormals[0]], 0);
			Vector3 transformedNormal1 = transform.MultiplyByVector3(normals[f.VertexNormals[1]], 0);
			Vector3 transformedNormal2 = transform.MultiplyByVector3(normals[f.VertexNormals[2]], 0);
			Vector2 uv1 = texCoords[f.VertexUVs[0]];
			Vector2 uv2 = texCoords[f.VertexUVs[1]];
			Vector2 uv3 = texCoords[f.VertexUVs[2]];
			_triangles.push_back(new Triangle(transformedPos0, transformedPos1, transformedPos2, transformedNormal0, transformedNormal1, transformedNormal2, uv1, uv2, uv3, material));
		}

		float minX, minY, minZ, maxX, maxY, maxZ;
		minX = minY = minZ = FLT_MAX;
		maxX = maxY = maxZ = -FLT_MAX;
		Vector3 center;
		size = (int)positions.size();
		for(int i = 0; i < size; ++i)
		{
			center += positions[i];
			minX = fmin(minX, positions[i][0]);
			minY = fmin(minY, positions[i][1]);
			minZ = fmin(minZ, positions[i][2]);
			maxX = fmax(maxX, positions[i][0]);
			maxY = fmax(maxY, positions[i][1]);
			maxZ = fmax(maxZ, positions[i][2]);
		}
		center /= (float) size;
		Vector3 boxSize = Vector3(maxX, maxY, maxZ) - Vector3(minX, minY, minZ);
		_boundingBox = new Box(center, boxSize, transform, nullptr);
	}

	Mesh::Mesh(const Mesh& other) : Shape(other)
	{
		int size = other._triangles.size();
		for(int i = 0; i < size; ++i)
		{
			_triangles.push_back(new Triangle(*other._triangles[i]));
		}
		_boundingBox = new Box(*other._boundingBox);
		_transform = other._transform;
	}

	Mesh::~Mesh()
	{}

	void Mesh::Init()
	{
		auto trianglesIt = _triangles.begin();
		auto trianglesEnd = _triangles.end();
		for(trianglesIt; trianglesIt != trianglesEnd; ++trianglesIt)
		{
			(*trianglesIt)->Init();
		}
		if(_boundingBox != nullptr)
		{
			_boundingBox->Init();
		}
	}

	void Mesh::Shutdown()
	{
		auto triangleIt = _triangles.begin();
		auto triangleEnd = _triangles.end();
		for(triangleIt; triangleIt != triangleEnd; ++triangleIt)
		{
			(*triangleIt)->Shutdown();
			delete (*triangleIt);
			(*triangleIt) = 0;
		}
	}

	bool Mesh::Intersects(const Ray& r, RaycastHit& hit) const
	{
		if(!r.Intersects(*_boundingBox, hit))
		{
			return false;
		}

		int size = _triangles.size();
		Triangle* closest = nullptr;
		float closestDistance = FLT_MAX;
		IntersectionPoint closestIP;
		for(int i = 0; i < size; ++i)
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