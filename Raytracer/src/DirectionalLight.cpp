#include "../include/DirectionalLight.h"

namespace raytracer
{
	DirectionalLight::DirectionalLight(Vector3 direction, Color24 color) : _direction(direction), Light(Vector3::Zero, color)
	{
		_direction.Normalize();
	}

	DirectionalLight::DirectionalLight(const DirectionalLight& other) : Light(other), _direction(other._direction)
	{
		_direction.Normalize();
	}

	Color24 DirectionalLight::Affect(Shape* shape, const IntersectionPoint& intersection, const Camera* camera, std::vector<Shape*>::iterator shapesBegin, std::vector<Shape*>::iterator shapesEnd) const
	{
		Ray r(intersection.Point, -_direction);
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
		float NdotL = fmax(Vector3::Dot(intersection.Normal, -_direction), 0.0f);
		Color24 diffuse = NdotL * _color * shape->GetMaterial()->GetDiffuse();

		//Specular calculations
		Vector3 V = (camera->GetPosition() - intersection.Point).Normalized();
		Vector3 H = (_direction + V).Normalized();
		float NdotH = clamp(-Vector3::Dot(intersection.Normal, H));
		Color24 specular = pow(NdotH, shape->GetMaterial()->GetShininess()) * shape->GetMaterial()->GetSpecular();
		
		return diffuse + specular;
	}
}