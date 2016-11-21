#pragma once

#include "../include/Texture.h"
#include "Material.h"

namespace raytracer
{
	class MaterialTexture : public Material
	{
	protected:
		Texture* _diffuseMap;

	public:
		MaterialTexture(const char* textureName = nullptr, Color24 specular = Color24::White, float shininess = 0.0f, float reflectance = 0.0f, float refraction = 0.0f);
		MaterialTexture(const MaterialTexture& other);

		virtual inline Color24 GetDiffuse(Vector2 uvs) const
		{
			return _diffuseMap->GetPixel(uvs[0], uvs[1]);
		}
	};
}