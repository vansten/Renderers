#include "../include/OBJLoader.h"

#include <fstream>

void OBJLoader::LoadOBJ(const char* fileName, std::vector<Vector3>& vertexPositions, std::vector<Vector2>& vertexUVs, std::vector<Vector3>& vertexNormals, std::vector<Face>& faces)
{
	std::string line;

	std::ifstream file;
	file.open(fileName, std::ios::in);
	if(file.is_open())
	{
		while(std::getline(file, line))
		{
			std::string v = line.substr(0, 2);
			if(v == "v ")
			{
				std::string vertex = line.substr(2);
				vertexPositions.push_back(ParseVector3(vertex));
			}
			else if(v == "vt")
			{
				std::string texCoords = line.substr(3);
				vertexUVs.push_back(ParseVector2(texCoords));
			}
			else if(v == "vn")
			{
				std::string normal = line.substr(3);
				vertexNormals.push_back(ParseVector3(normal));
			}
			else if(v == "f ")
			{
				std::string face = line.substr(2);
				faces.push_back(ParseFace(face));
			}
		}
		file.close();
	}
}

Face OBJLoader::ParseFace(std::string face)
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

	f.VertexPositions = Int3(v1, v2, v3);
	f.VertexNormals = Int3(vn1, vn2, vn3);
	f.VertexUVs = Int3(vt1, vt2, vt3);

	return f;
}

Vector3 OBJLoader::ParseVector3(std::string line)
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

Vector2 OBJLoader::ParseVector2(std::string line)
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
