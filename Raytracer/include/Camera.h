#pragma once

#include "Ray.h"
#include "Pixel.h"

namespace raytracer
{
	class Camera
	{
	protected:
		Vector3 _position;
		float _aspectRatio;
		float _onePerAspectRatio;

	public:
		Camera(Vector3 position = Vector3::Zero);
		Camera(const Camera& other);
		virtual ~Camera();

		virtual inline Vector3 GetPosition() const
		{
			return _position;
		}

		virtual void ConstructRay(Ray& r, const Pixel& p) const = 0;
	};
}