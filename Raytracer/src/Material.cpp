#include "../include/Material.h"

namespace raytracer
{
	Material::Material(Color24 diffuse, Color24 specular, float shininess, float reflectance, float refraction) : _diffuse(diffuse), _specular(specular), _shininess(shininess), _refraction(refraction), _reflectance(reflectance)
	{
		float both = _refraction + _reflectance;
		if(both > epsilon)
		{
			_refraction /= both;
			_reflectance /= both;
		}
	}

	Material::Material(const Material& other) : _diffuse(other._diffuse), _specular(other._specular), _shininess(other._shininess), _refraction(other._refraction), _reflectance(other._reflectance)
	{}
}