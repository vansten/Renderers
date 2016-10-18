#pragma once

#include <Windows.h>

class Timer
{
protected:
	double _frequency;
	LARGE_INTEGER _timerStart;

public:
	Timer();
	~Timer();

	void Start();
	double GetElapsedTime();
};

