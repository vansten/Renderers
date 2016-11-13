#include "../include/Light.h"
#include "../include/Console.h"

namespace raytracer
{
	bool Light::IsObscured(const Shape* shape, const Vector3& origin, const Vector3& direction, const std::vector<Shape*>::iterator shapesBegin, const std::vector<Shape*>::iterator shapesEnd) const
	{
		Ray r(origin, direction);
		RaycastHit hit;
		bool intersects = false;
		for(auto shapesIt = shapesBegin; shapesIt != shapesEnd; ++shapesIt)
		{
			if((*shapesIt) != shape)
			{
				if(r.Intersects(*(*shapesIt), hit))
				{
					return true;
				}
			}
		}

		return false;
	}

	Color24 Light::CalculateDiffuse(const IntersectionPoint& intersection, const Material* material, const Vector3& L) const
	{
		if(material == nullptr)
		{
			return Color24::Black;
		}

		float NdotL = clamp(Vector3::Dot(L, intersection.Normal));
		return NdotL * _color * material->GetDiffuse(intersection.UVs);
	}

	Color24 Light::CalculateSpecular(const Camera* camera, const IntersectionPoint& intersection, const Material* material, const Vector3& L) const
	{
		if(camera == nullptr || material == nullptr)
		{
			return Color24::Black;
		}

		Vector3 V = (camera->GetPosition() - intersection.Point).Normalized();
		Vector3 H = (L + V).Normalized();
		float NdotH = clamp(Vector3::Dot(intersection.Normal, H));
		return pow(NdotH, material->GetShininess(intersection.UVs)) * material->GetSpecular(intersection.UVs);
	}
}