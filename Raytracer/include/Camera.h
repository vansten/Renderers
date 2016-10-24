#pragma once

#include "Ray.h"
#include "Pixel.h"

namespace raytracer
{
	class Camera
	{
	protected:
		float _aspectRatio;
		float _onePerAspectRatio;

	public:
		Camera();
		Camera(const Camera& other);
		virtual ~Camera();

		virtual void ConstructRay(Ray& r, const Pixel& p) const = 0;
	};
}