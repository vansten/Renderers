#pragma once

#include "Types.h"
#include "Shape.h"
#include "Camera.h"

namespace raytracer
{
	class Light
	{
	protected:
		Vector3 _position;
		Color24 _color;

	public:
		Light(Vector3 position, Color24 color) : _position(position), _color(color) {}
		Light(const Light& other) : _position(other._position), _color(other._color) {}

		virtual Color24 Affect(Shape* shape, const IntersectionPoint& intersection, const Camera* camera, std::vector<Shape*>::iterator shapesBegin, std::vector<Shape*>::iterator shapesEnd) const = 0;
	};
}