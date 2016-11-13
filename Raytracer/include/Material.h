#pragma once

#include "../include/Types.h"
#include "../include/Vector.h"

namespace raytracer
{
	class Material
	{
	protected:
		Color24 _diffuse;
		Color24 _specular;
		float _shininess;

	public:
		Material(Color24 diffuse = Color24::White, Color24 specular = Color24::White, float shininess = 0.0f);
		Material(const Material& other);

		virtual inline Color24 GetDiffuse(Vector2 uvs) const
		{
			return _diffuse;
		}

		virtual inline Color24 GetSpecular(Vector2 uvs) const
		{
			return _specular;
		}

		virtual inline float GetShininess(Vector2 uvs) const
		{
			return _shininess;
		}
	};
}