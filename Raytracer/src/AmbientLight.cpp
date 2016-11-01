#include "../include/AmbientLight.h"

namespace raytracer
{
	AmbientLight::AmbientLight(Color24 color) : Light(Vector3::Zero, color)
	{

	}

	AmbientLight::AmbientLight(const AmbientLight& other) : Light(other)
	{

	}

	Color24 AmbientLight::Affect(Shape* shape, const IntersectionPoint& intersection, std::vector<Shape*>::iterator shapesBegin, std::vector<Shape*>::iterator shapesEnd) const
	{
		return _color;
	}
}