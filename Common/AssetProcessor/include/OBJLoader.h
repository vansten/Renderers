#pragma once

#include <vector>

#include "Vector.h"
#include "Int3.h"

class Face
{
public:
	Int3 VertexPositions;
	Int3 VertexNormals;
	Int3 VertexUVs;
};

class OBJLoader
{
public:
	static void LoadOBJ(const char* fileName, std::vector<Vector3>& vertexPositions, std::vector<Vector2>& vertexUVs, std::vector<Vector3>& vertexNormals, std::vector<Face>& faces);

protected:
	static Face ParseFace(std::string face);
	static Vector3 ParseVector3(std::string line);
	static Vector2 ParseVector2(std::string line);
};

