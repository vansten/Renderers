#include "../include/Mesh.h"

#include "OBJLoader.h"

namespace raytracer
{
	Mesh::Mesh(const char* filename, const Matrix& transform, Material* material) : _transform(transform)
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
			_triangles.push_back(new Triangle(transformedPos0, transformedPos1, transformedPos2, material));
		}
	}


	Mesh::~Mesh()
	{}

	bool Mesh::Init()
	{
		auto trianglesIt = _triangles.begin();
		auto trianglesEnd = _triangles.end();
		for(trianglesIt; trianglesIt != trianglesEnd; ++trianglesIt)
		{
			(*trianglesIt)->Init();
		}
		return true;
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
}