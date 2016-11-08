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

		virtual Color24 Affect(const Shape* shape, const IntersectionPoint& intersection, const Camera* camera, const std::vector<Shape*>::iterator shapesBegin, const std::vector<Shape*>::iterator shapesEnd) const = 0;
	
	protected:
		virtual bool IsObscured(const Shape* shape, const Vector3& origin, const Vector3& direction, const std::vector<Shape*>::iterator shapesBegin, const std::vector<Shape*>::iterator shapesEnd) const;
		virtual Color24 CalculateDiffuse(const IntersectionPoint& intersection, const Material* material, const Vector3& L) const;
		virtual Color24 CalculateSpecular(const Camera* camera, const IntersectionPoint& intersection, const Material* material, const Vector3& L) const;
	};
}