#pragma once

#include "Camera.h"

namespace raytracer
{
	class PerspectiveCamera : public Camera
	{
	protected:
		float _fov;
		float _tanFovDiv2;
		float _near;
		float _far;

	public:
		PerspectiveCamera(float fov, float nearPlane, float farPlane);
		PerspectiveCamera(const PerspectiveCamera& other);

		virtual void ConstructRay(Ray& r, const Pixel& p) const override;
	};
}