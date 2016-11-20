#include "../include/Shape.h"

namespace raytracer
{
	Shape::Shape(Material* material) : _material(material), _calculateLights(true)
	{

	}

	Shape::Shape(const Shape& other) : _material(other._material), _calculateLights(other._calculateLights)
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

	void Shape::SetCalculateLights(bool calculate)
	{
		_calculateLights = calculate;
	}

	bool Shape::Intersects(const Ray& r, RaycastHit& hit) const
	{
		return false;
	}
}