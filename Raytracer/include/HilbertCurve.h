#pragma once

#include "../include/Vector.h"

namespace raytracer
{
	class HilbertCurve
	{
	public:
		static Vector2 Evaluate(int max, int i);

	protected:
		static Vector2 Rotate(int n, Vector2 prev, int rx, int ry);
	};
}