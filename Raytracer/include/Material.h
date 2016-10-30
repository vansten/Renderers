#pragma once

#include "../include/Types.h"

class Material
{
protected:
	Color24 _diffuse;
	Color24 _specular;
	float _shininess;

public:
	Material(Color24 diffuse = Color24::White, Color24 specular = Color24::White, float shininess = 0.0f);
	Material(const Material& other);
	
	inline Color24 GetDiffuse()
	{
		return _diffuse;
	}

	inline Color24 GetSpecular()
	{
		return _specular;
	}

	inline float GetShininess()
	{
		return _shininess;
	}
};