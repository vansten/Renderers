#include "../include/PointLight.h"

namespace raytracer
{
	PointLight::PointLight(Vector3 position, Color24 color, float intensity, float attenuation) : Light(position, color), _intensity(intensity), _attenuation(attenuation)
	{

	}

	PointLight::PointLight(const PointLight& other) : Light(other), _intensity(other._intensity), _attenuation(other._attenuation)
	{

	}

	Color24 PointLight::Affect(Shape* shape, const IntersectionPoint& intersection, std::vector<Shape*>::iterator shapesBegin, std::vector<Shape*>::iterator shapesEnd) const
	{
		Ray r(intersection.Point, _position - intersection.Point);
		RaycastHit hit;
		bool intersects = false;
		for(auto shapesIt = shapesBegin; shapesIt != shapesEnd; ++shapesIt)
		{
			if((*shapesIt) != shape)
			{
				if(r.Intersects(*(*shapesIt), hit))
				{
					intersects = true;
					break;
				}
			}
		}

		if(intersects)
		{
			return Color24::Black;
		}

		float NdotL = clamp(Vector3::Dot(r.Direction, intersection.Normal));
		float length = (_position - intersection.Point).Length();
		float att = 1.0f / (_attenuation + _attenuation * length);
		return NdotL * att * _intensity * _color * shape->GetMaterial()->GetDiffuse();
	}
}