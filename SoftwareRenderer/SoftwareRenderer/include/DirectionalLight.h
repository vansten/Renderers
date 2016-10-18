#pragma once

#include "Vector.h"
#include "Types.h"

class DirectionalLight
{
public:
	Vector3 Direction;
	Color32 Color;

public:
	DirectionalLight(Vector3 direction, Color32 color);
	~DirectionalLight();
};

