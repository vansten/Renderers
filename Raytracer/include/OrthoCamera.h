#pragma once

#include "Camera.h"

namespace raytracer
{
	class OrthoCamera : public Camera
	{
	protected:
		float _size;

	public:
		OrthoCamera(float size, Vector3 position = Vector3::Zero);
		OrthoCamera(const OrthoCamera& other);

		virtual void ConstructRay(Ray& r, const Pixel& p) const override;
	};
}