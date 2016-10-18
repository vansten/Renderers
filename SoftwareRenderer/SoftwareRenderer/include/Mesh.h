#pragma once

#include "Shape.h"

#include "Texture.h"

class Mesh : public Shape
{
private:
	class Face
	{
	public:
		Int3 vertices;
		Int3 normals;
		Int3 uvs;
	};

protected:
	Texture* _texture;

public:
	Mesh();
	Mesh(std::string fileName, std::string textureName, TextureWrapMode wrapMode = TextureWrapMode::Clamp);
	virtual ~Mesh();

	virtual void Draw(DeviceContext* deviceContext, DirectionalLight* light) override;

protected:
	void LoadFromOBJ(std::string fileName);
	Face ParseFace(std::string face);
	Vector3 ParseVector3(std::string line);
	Vector2 ParseVector2(std::string line);
};