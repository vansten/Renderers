#pragma once

#include "Types.h"
#include "Shape.h"

namespace raytracer
{
	class Light
	{
	public:
		Vector3 Position;

	public:
		virtual Color24 Affect(Shape* shape, const IntersectionPoint& intersection, std::vector<Shape*>::iterator shapesBegin, std::vector<Shape*>::iterator shapesEnd) const = 0;
	};
}