#pragma once

#include "Light.h"

namespace raytracer
{
	class PointLight : public Light
	{
	protected:
		float _intensity;
		float _attenuation;

	public:
		PointLight(Vector3 position, Color24 color, float intenstity, float attenuation);
		PointLight(const PointLight& other);

		virtual Color24 Affect(const Shape* shape, const IntersectionPoint& intersection, const Camera* camera, const std::vector<Shape*>::iterator shapesBegin, const std::vector<Shape*>::iterator shapesEnd) const override;
	};
}