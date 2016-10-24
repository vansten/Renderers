#pragma once

#include "Camera.h"

namespace raytracer
{
	class OrthoCamera : public Camera
	{
	protected:
		float _size;
		float _cameraZ;

	public:
		OrthoCamera(float size, float cameraZ);
		OrthoCamera(const OrthoCamera& other);

		virtual void ConstructRay(Ray& r, const Pixel& p) const override;
	};
}