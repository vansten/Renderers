#include "../include/Mesh.h"
#include "Console.h"

#include "../include/DeviceContext.h"

#include <fstream>
#include "TGASerializer.h"

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
			TGASerializer::SaveTGA("textures/test.tga", _texture);
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

void Mesh::Draw(DeviceContext* deviceContext, DirectionalLight* light)
{
	deviceContext->SetTexture(_texture);
	deviceContext->DrawIndexed(_modelMatrix, _vertices, _indices, light);
}

void Mesh::LoadFromOBJ(std::string fileName)
{
	std::string line;

	std::vector<Vector3> readVertices;
	std::vector<Vector3> readNormals;
	std::vector<Vector2> readTexCoords;
	std::vector<Face> readFaces;
	std::ifstream file;
	file.open(fileName.c_str(), std::ios::in);
	if(file.is_open())
	{
		while(std::getline(file, line))
		{
			std::string v = line.substr(0, 2);
			if(v == "v ")
			{
				std::string vertex = line.substr(2);
				readVertices.push_back(ParseVector3(vertex));
			}
			else if(v == "vt")
			{
				std::string texCoords = line.substr(3);
				readTexCoords.push_back(ParseVector2(texCoords));
			}
			else if(v == "vn")
			{
				std::string normal = line.substr(3);
				readNormals.push_back(ParseVector3(normal));
			}
			else if(v == "f ")
			{
				std::string face = line.substr(2);
				readFaces.push_back(ParseFace(face));
			}
		}
		file.close();
	}

	auto it = readFaces.begin();
	auto end = readFaces.end();
	int i = 0;
	for(it; it != end; ++it)
	{
		Face f = (*it);
		Vector3 v1, v2, v3;
		Vector3 vn1, vn2, vn3;
		Vector2 vt1, vt2, vt3;
		v1 = readVertices[f.vertices[0]];
		v2 = readVertices[f.vertices[1]];
		v3 = readVertices[f.vertices[2]];
		vn1 = readNormals[f.normals[0]];
		vn2 = readNormals[f.normals[1]];
		vn3 = readNormals[f.normals[2]];
		vt1 = readTexCoords[f.uvs[0]];
		vt2 = readTexCoords[f.uvs[1]];
		vt3 = readTexCoords[f.uvs[2]];

		_vertices.push_back(VertexPositionUVNormal(v1, vn1, vt1));
		_vertices.push_back(VertexPositionUVNormal(v2, vn2, vt2));
		_vertices.push_back(VertexPositionUVNormal(v3, vn3, vt3));
		_indices.push_back(Int3(i, i + 1, i + 2));
		i += 3;
	}
}

Mesh::Face Mesh::ParseFace(std::string face)
{
	Face f;
	std::string vS1, vtS1, vnS1;
	std::string vS2, vtS2, vnS2;
	std::string vS3, vtS3, vnS3;
	int v1, v2, v3;
	int vt1, vt2, vt3;
	int vn1, vn2, vn3;
	int i = 0;

	//First vertex
	while(face[i] != '/')
	{
		vS1 += face[i];
		++i;
	}
	v1 = std::stoi(vS1) - 1;
	++i;
	while(face[i] != '/')
	{
		vtS1 += face[i];
		++i;
	}
	vt1 = std::stoi(vtS1) - 1;
	++i;
	while(face[i] != ' ')
	{
		vnS1 += face[i];
		++i;
	}
	vn1 = std::stoi(vnS1) - 1;

	//Second vertex
	while(face[i] != '/')
	{
		vS2 += face[i];
		++i;
	}
	v2 = std::stoi(vS2) - 1;
	++i;
	while(face[i] != '/')
	{
		vtS2 += face[i];
		++i;
	}
	vt2 = std::stoi(vtS2) - 1;
	++i;
	while(face[i] != ' ')
	{
		vnS2 += face[i];
		++i;
	}
	vn2 = std::stoi(vnS2) - 1;

	//Third vertex
	while(face[i] != '/')
	{
		vS3 += face[i];
		++i;
	}
	v3 = std::stoi(vS3) - 1;
	++i;
	while(face[i] != '/')
	{
		vtS3 += face[i];
		++i;
	}
	vt3 = std::stoi(vtS3) - 1;
	++i;
	while(i < face.size())
	{
		vnS3 += face[i];
		++i;
	}
	vn3 = std::stoi(vnS3) - 1;
	
	f.vertices = Int3(v1, v2, v3);
	f.normals = Int3(vn1, vn2, vn3);
	f.uvs = Int3(vt1, vt2, vt3);

	return f;
}

Vector3 Mesh::ParseVector3(std::string line)
{
	int i = 0;
	std::string xS = "";
	std::string yS = "";
	std::string zS = "";
	while(line[i] != ' ')
	{
		xS += line[i];
		++i;
	}
	++i;
	while(line[i] != ' ')
	{
		yS += line[i];
		++i;
	}
	++i;
	while(i < line.size())
	{
		zS += line[i];
		++i;
	}
	float x = std::stof(xS);
	float y = std::stof(yS);
	float z = std::stof(zS);
	return Vector3(x, y, z);
}

Vector2 Mesh::ParseVector2(std::string line)
{
	int i = 0;
	std::string xS = "";
	std::string yS = "";
	while(line[i] != ' ')
	{
		xS += line[i];
		++i;
	}
	++i;
	while(i < line.size())
	{
		yS += line[i];
		++i;
	}
	float x = std::stof(xS);
	float y = std::stof(yS);
	return Vector2(x, y);
}
