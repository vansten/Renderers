#pragma once

#include "Light.h"

namespace raytracer
{
	class AmbientLight : public Light
	{
	public:
		AmbientLight(Color24 color);
		AmbientLight(const AmbientLight& other);

		virtual Color24 Affect(Shape* shape, const IntersectionPoint& intersection, std::vector<Shape*>::iterator shapesBegin, std::vector<Shape*>::iterator shapesEnd) const override;
	};
}