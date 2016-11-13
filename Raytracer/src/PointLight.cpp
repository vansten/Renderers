#include "../include/PointLight.h"

namespace raytracer
{
	PointLight::PointLight(Vector3 position, Color24 color, float intensity, float attenuation) : Light(position, color), _intensity(intensity), _attenuation(attenuation)
	{

	}

	PointLight::PointLight(const PointLight& other) : Light(other), _intensity(other._intensity), _attenuation(other._attenuation)
	{

	}

	Color24 PointLight::Affect(const Shape* shape, const IntersectionPoint& intersection, const Camera* camera, const std::vector<Shape*>::iterator shapesBegin, const std::vector<Shape*>::iterator shapesEnd) const
	{
		Vector3 L = _position - intersection.Point;
		
		if(IsObscured(shape, intersection.Point, L, shapesBegin, shapesEnd))
		{
			return Color24::Black;
		}

		float length = L.Length();
		L.Normalize();
		float att = 1.0f / (_attenuation + _attenuation * length);
		const Material* material = shape->GetMaterial();
		
		return att * (CalculateDiffuse(intersection, material, L) * _intensity + CalculateSpecular(camera, intersection, material, L));
	}

	bool PointLight::IsObscured(const Shape* shape, const Vector3& origin, const Vector3& direction, const std::vector<Shape*>::iterator shapesBegin, const std::vector<Shape*>::iterator shapesEnd) const
	{
		float squaredLength = direction.LengthSquared();
		Ray r(origin, direction);
		RaycastHit hit;
		bool intersects = false;
		for(auto shapesIt = shapesBegin; shapesIt != shapesEnd; ++shapesIt)
		{
			if((*shapesIt) != shape)
			{
				if(r.Intersects(*(*shapesIt), hit))
				{
					std::vector<IntersectionPoint> intersections = hit.GetIntersectionPoints();
					int hitsCount = intersections.size();
					for(int i = 0; i < hitsCount; ++i)
					{
						if((intersections[0].Point - _position).LengthSquared() < squaredLength)
						{
							return true;
						}
					}
				}
			}
		}

		return false;
	}
}