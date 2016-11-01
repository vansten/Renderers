#include "../include/DirectionalLight.h"

namespace raytracer
{
	DirectionalLight::DirectionalLight(Vector3 position, Vector3 direction, Color24 color) : _direction(direction), _color(color)
	{
		Position = position;
		_direction.Normalize();
	}

	Color24 DirectionalLight::Affect(Shape* shape, const IntersectionPoint& intersection, std::vector<Shape*>::iterator shapesBegin, std::vector<Shape*>::iterator shapesEnd) const
	{
		Ray r(intersection.Point, -_direction);
		RaycastHit hit;
		bool intersects = false;
		for(auto shapesIt = shapesBegin; shapesIt != shapesEnd; ++shapesIt)
		{
			if((*shapesIt) != shape)
			{
				if(r.Intersects(*(*shapesIt), hit))
				{
					intersects = true;
					break;
				}
			}
		}
		if(intersects)
		{
			return Color24::Black;
		}

		float NdotL = fmax(Vector3::Dot(intersection.Normal, -_direction), 0.0f);
		return NdotL * _color * shape->GetMaterial()->GetDiffuse();
	}
}