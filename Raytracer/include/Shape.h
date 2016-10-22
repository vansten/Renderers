#pragma once

#include "Ray.h"
#include "RaycastHit.h"
#include "../include/Types.h"

namespace raytracer
{
	class Shape
	{
		friend class Ray;

	public:
		Color24 Color;

	public:
		Shape(Color24 color = Color24::Magenta);
		Shape(const Shape& other);
		virtual ~Shape();
		virtual void Init();
		virtual void Shutdown();
		virtual std::string ToString() const;
		virtual Color24 GetColor() const;

	protected:
		virtual bool Intersects(const Ray& r, RaycastHit& hit) const;
	};
}