#pragma once

#include "Light.h"

namespace raytracer
{
	class DirectionalLight : public Light
	{
	protected:
		Vector3 _direction;
		Color24 _color;

	public:
		DirectionalLight(Vector3 position, Vector3 direction, Color24 color);

		virtual Color24 Affect(Shape* shape, const IntersectionPoint& intersection, std::vector<Shape*>::iterator shapesBegin, std::vector<Shape*>::iterator shapesEnd) const override;
	};
}