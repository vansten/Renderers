#include "../include/PointLight.h"

namespace raytracer
{
	PointLight::PointLight(Vector3 position, Color24 color, float intensity, float attenuation) : Light(position, color), _intensity(intensity), _attenuation(attenuation)
	{

	}

	PointLight::PointLight(const PointLight& other) : Light(other), _intensity(other._intensity), _attenuation(other._attenuation)
	{

	}

	Color24 PointLight::Affect(Shape* shape, const IntersectionPoint& intersection, const Camera* camera, std::vector<Shape*>::iterator shapesBegin, std::vector<Shape*>::iterator shapesEnd) const
	{
		Vector3 L = _position - intersection.Point;
		float length = L.Length();
		L.Normalize();
		Ray r(intersection.Point, L);
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

		//Diffuse calculations
		float NdotL = clamp(Vector3::Dot(r.Direction, intersection.Normal));
		float att = 1.0f / (_attenuation + _attenuation * length);
		Color24 diffuse = NdotL * att * _intensity * _color * shape->GetMaterial()->GetDiffuse();

		//Specular calculations
		Vector3 V = (camera->GetPosition() - intersection.Point).Normalized();
		Vector3 H = (L + V).Normalized();
		float NdotH = clamp(Vector3::Dot(intersection.Normal, H));
		Color24 specular = att * pow(NdotH, shape->GetMaterial()->GetShininess()) * shape->GetMaterial()->GetSpecular();

		return diffuse + specular;
	}
}