#include "../include/DirectionalLight.h"

namespace raytracer
{
	DirectionalLight::DirectionalLight(Vector3 direction, Color24 color) : _direction(direction), Light(Vector3::Zero, color)
	{
		_direction.Normalize();
	}

	DirectionalLight::DirectionalLight(const DirectionalLight& other) : Light(other), _direction(other._direction)
	{
		_direction.Normalize();
	}

	Color24 DirectionalLight::Affect(const Shape* shape, const IntersectionPoint& intersection, const Camera* camera, const std::vector<Shape*>::iterator shapesBegin, const std::vector<Shape*>::iterator shapesEnd) const
	{
		Vector3 L = -_direction;

		if(IsObscured(shape, intersection.Point, L, shapesBegin, shapesEnd))
		{
			return Color24::Black;
		}

		const Material* material = shape->GetMaterial();

		return CalculateDiffuse(intersection, material, L) + CalculateSpecular(camera, intersection, material, L);
	}
}