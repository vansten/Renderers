#include "../include/AmbientLight.h"

namespace raytracer
{
	AmbientLight::AmbientLight(Color24 color) : Light(Vector3::Zero, color)
	{

	}

	AmbientLight::AmbientLight(const AmbientLight& other) : Light(other)
	{

	}

	Color24 AmbientLight::Affect(const Shape* shape, const IntersectionPoint& intersection, const Camera* camera, const std::vector<Shape*>::iterator shapesBegin, const std::vector<Shape*>::iterator shapesEnd) const
	{
		return _color;
	}
}