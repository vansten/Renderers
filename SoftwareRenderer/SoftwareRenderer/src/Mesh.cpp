#include "../include/Mesh.h"
#include "Console.h"

#include "../include/DeviceContext.h"

#include <fstream>
#include "TGASerializer.h"

#include "OBJLoader.h"

Mesh::Mesh() : Shape()
{

}

Mesh::Mesh(std::string fileName, std::string textureName, TextureWrapMode wrapMode, TextureFiltering filtering) : Shape()
{
	//Get extension
	//If extension == obj then LoadObj();
	std::string extension = fileName.substr(fileName.size() - 4, 4);
	if(extension == ".obj")
	{
		LoadFromOBJ(fileName);
	}
	else
	{
		Console::WriteLine("Unrecognized mesh format");
	}
	extension = textureName.substr(textureName.size() - 4, 4);
	if(extension == ".tga")
	{
		TGASerializer::LoadTGA(textureName.c_str(), &_texture);
		if(_texture != 0)
		{
			_texture->SetWrapMode(wrapMode);
			_texture->SetFiltering(filtering);
		}
	}
	else
	{
		Console::WriteLine("Unrecognized texture format");
	}

	if(_texture)
	{
		_texture->SetWrapMode(wrapMode);
	}
}

Mesh::~Mesh()
{
	if(_texture)
	{
		delete _texture;
		_texture = 0;
	}
}

void Mesh::Draw(DeviceContext* deviceContext, const DirectionalLight* light, const SpotLight* spotlight)
{
	deviceContext->SetTexture(_texture);
	deviceContext->DrawIndexed(_modelMatrix, _vertices, _indices, light, spotlight);
}

void Mesh::AddToUVs(float u, float v)
{
	int verticesSize = (int)_vertices.size();
	for(int i = 0; i < verticesSize; ++i)
	{
		_vertices[i].UV += Vector2(u, v);
	}
}

void Mesh::LoadFromOBJ(std::string fileName)
{
	std::vector<Vector3> readVertices;
	std::vector<Vector3> readNormals;
	std::vector<Vector2> readUVs;
	std::vector<Face> readFaces;

	OBJLoader::LoadOBJ(fileName.c_str(), readVertices, readUVs, readNormals, readFaces);

	auto it = readFaces.begin();
	auto end = readFaces.end();
	int i = 0;
	for(it; it != end; ++it)
	{
		Face f = (*it);
		Vector3 v1, v2, v3;
		Vector3 vn1, vn2, vn3;
		Vector2 vt1, vt2, vt3;
		v1 = readVertices[f.VertexPositions[0]];
		v2 = readVertices[f.VertexPositions[1]];
		v3 = readVertices[f.VertexPositions[2]];
		vn1 = readNormals[f.VertexNormals[0]];
		vn2 = readNormals[f.VertexNormals[1]];
		vn3 = readNormals[f.VertexNormals[2]];
		vt1 = readUVs[f.VertexUVs[0]];
		vt2 = readUVs[f.VertexUVs[1]];
		vt3 = readUVs[f.VertexUVs[2]];

		_vertices.push_back(VertexPositionUVNormal(v1, vn1, vt1));
		_vertices.push_back(VertexPositionUVNormal(v2, vn2, vt2));
		_vertices.push_back(VertexPositionUVNormal(v3, vn3, vt3));
		_indices.push_back(Int3(i, i + 1, i + 2));
		i += 3;
	}
}