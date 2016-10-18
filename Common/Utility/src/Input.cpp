#include "../include/Input.h"

Input::Input()
{
}

Input::~Input()
{
}

void Input::Init()
{
	HMODULE xInputLibrary = LoadLibrary("Xinput1_4.dll");
	if(!xInputLibrary)
	{
		xInputLibrary = LoadLibrary("Xinput1_3.dll");
	}

	if(xInputLibrary)
	{
		_getState = (GetState)GetProcAddress(xInputLibrary, "XInputGetState");
	}
}

void Input::Update()
{
	if(_getState)
	{
		XINPUT_STATE s;
		_getState(0, &s);
		_leftStick.Set(0, s.Gamepad.sThumbLX / (float)0xccc);
		_leftStick.Set(1, s.Gamepad.sThumbLY / (float)0xccc);
		_rightStick.Set(0, s.Gamepad.sThumbRX / (float)0xccc);
		_rightStick.Set(1, s.Gamepad.sThumbRY / (float)0xccc);
	}
}

Vector2 Input::GetLeftStick()
{
	return Vector2::Zero;
	//return _leftStick;
}

Vector2 Input::GetRightStick()
{
	return _rightStick;
}