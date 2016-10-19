#include "../include/SpotLight.h"

SpotLight::SpotLight(Vector3 position, Vector3 direction, Color32 color, Vector2 attenuation, float cutoffDegree, float exp) :
	Position(position),
	Direction(direction),
	Color(color),
	Attenuation(attenuation), 
	SpotCutoffCosinus(cos(cutoffDegree * Deg2Rad)), 
	SpotExponent(exp)
{
	Direction.Normalize();
}
