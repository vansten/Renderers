#include "../include/Material.h"

namespace raytracer
{
	Material::Material(Color24 diffuse, Color24 specular, float shininess) : _diffuse(diffuse), _specular(specular), _shininess(shininess)
	{}

	Material::Material(const Material& other) : _diffuse(other._diffuse), _specular(other._specular), _shininess(other._shininess)
	{}
}