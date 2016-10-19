#pragma once

#include "Shape.h"

#include "Texture.h"

class Mesh : public Shape
{
protected:
	Texture* _texture;

public:
	Mesh();
	Mesh(std::string fileName, std::string textureName, TextureWrapMode wrapMode = TextureWrapMode::Clamp, TextureFiltering fitering = TextureFiltering::Nearest);
	virtual ~Mesh();

	virtual void Draw(DeviceContext* deviceContext, const DirectionalLight* light, const SpotLight* spotlight) override;

	void AddToUVs(float u, float v);

protected:
	void LoadFromOBJ(std::string fileName);
};