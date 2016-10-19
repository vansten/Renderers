#pragma once

#include "Vector.h"

class SpotLight
{
public:
	Color32 Color;
	Vector3 Position;
	Vector3 Direction;
	Vector2 Attenuation;
	float SpotCutoffCosinus;
	float SpotExponent;

public:
	SpotLight(Vector3 position, Vector3 Direction, Color32 color, Vector2 attenuation, float cutoffDegree, float exp);
};