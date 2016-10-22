#pragma once

#include <vector>
#include "Vector.h"

namespace raytracer
{
	class RaycastHit
	{
	protected:
		std::vector<Vector3> _intersectionPoints;

	public:
		RaycastHit();
		virtual ~RaycastHit();

		void AddIntersectionPoint(const Vector3& p);
		const std::vector<Vector3>& GetIntersectionPoints();
		void Clear();
	};
}