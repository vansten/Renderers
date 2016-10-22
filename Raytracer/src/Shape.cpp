#include "../include/Shape.h"

namespace raytracer
{
	Shape::Shape(Color24 color) : Color(color)
	{

	}

	Shape::Shape(const Shape& other) : Color(other.Color)
	{

	}

	Shape::~Shape()
	{

	}

	void Shape::Init()
	{

	}

	void Shape::Shutdown()
	{

	}

	std::string Shape::ToString() const
	{
		return "Shape";
	}

	Color24 Shape::GetColor() const
	{
		return Color;
	}

	bool Shape::Intersects(const Ray& r, RaycastHit& hit) const
	{
		return false;
	}
}