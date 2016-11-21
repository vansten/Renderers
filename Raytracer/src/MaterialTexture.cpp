#include "../include/MaterialTexture.h"
#include "../include/TGASerializer.h"

namespace raytracer
{
	MaterialTexture::MaterialTexture(const char* textureName, Color24 specular, float shininess, float reflectance, float refraction) : Material(Color24::White, specular, shininess, reflectance, refraction), _diffuseMap(nullptr)
	{
		TGASerializer::LoadTGA(textureName, &_diffuseMap);
	}

	MaterialTexture::MaterialTexture(const MaterialTexture& other) : Material(other), _diffuseMap(nullptr)
	{
		_diffuseMap = new Texture(*other._diffuseMap);
	}
}