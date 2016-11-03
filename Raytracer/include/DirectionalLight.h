#pragma once

#include "Light.h"

namespace raytracer
{
	class DirectionalLight : public Light
	{
	protected:
		Vector3 _direction;

	public:
		DirectionalLight(Vector3 direction, Color24 color);
		DirectionalLight(const DirectionalLight& other);

		virtual Color24 Affect(Shape* shape, const IntersectionPoint& intersection, const Camera* camera, std::vector<Shape*>::iterator shapesBegin, std::vector<Shape*>::iterator shapesEnd) const override;
	};
}