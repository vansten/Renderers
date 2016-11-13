#pragma once

#include <vector>
#include "Vector.h"

namespace raytracer
{
	struct IntersectionPoint
	{
	public:
		IntersectionPoint(Vector3 point = Vector3::Zero, Vector3 normal = Vector3::Zero, Vector2 uvs = Vector2::Zero);

		Vector3 Point;
		Vector3 Normal;
		Vector2 UVs;
	};

	class RaycastHit
	{
	protected:
		std::vector<IntersectionPoint> _intersectionPoints;

	public:
		RaycastHit();
		virtual ~RaycastHit();

		void AddIntersectionPoint(const IntersectionPoint& p);
		const std::vector<IntersectionPoint>& GetIntersectionPoints();
		void Clear();
	};
}