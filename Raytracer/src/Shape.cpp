#include "../include/Shape.h"

namespace raytracer
{
	Shape::Shape(Material* material) : _material(material)
	{

	}

	Shape::Shape(const Shape& other) : _material(other._material)
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

	void Shape::SetMaterial(Material * material)
	{
		_material = material;
	}

	bool Shape::Intersects(const Ray& r, RaycastHit& hit) const
	{
		return false;
	}
}