#pragma once

#include <Windows.h>
#include <Xinput.h>

#include "Vector.h"

typedef DWORD(*GetState)(DWORD userIndex, XINPUT_STATE* state);

class Input
{
protected:
	Vector2 _leftStick;
	Vector2 _rightStick;

	GetState _getState;

public:
	Input();
	~Input();

	void Init();
	void Update();
	
	Vector2 GetLeftStick();
	Vector2 GetRightStick();
};