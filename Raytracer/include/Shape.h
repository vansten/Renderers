#pragma once

#include "Ray.h"
#include "RaycastHit.h"
#include "../include/MaterialTexture.h"

namespace raytracer
{
	class Shape
	{
		friend class Ray;

	protected:
		Material* _material;

	public:
		Shape(Material* material = nullptr);
		Shape(const Shape& other);
		virtual ~Shape();
		virtual void Init();
		virtual void Shutdown();
		virtual std::string ToString() const;

		void SetMaterial(Material* material);
		inline const Material* GetMaterial() const
		{
			return _material;
		}

	protected:
		virtual bool Intersects(const Ray& r, RaycastHit& hit) const;
	};
}