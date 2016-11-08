#pragma once

#include "Light.h"

namespace raytracer
{
	class AmbientLight : public Light
	{
	public:
		AmbientLight(Color24 color);
		AmbientLight(const AmbientLight& other);

		virtual Color24 Affect(const Shape* shape, const IntersectionPoint& intersection, const Camera* camera, const std::vector<Shape*>::iterator shapesBegin, const std::vector<Shape*>::iterator shapesEnd) const override;
	};
}