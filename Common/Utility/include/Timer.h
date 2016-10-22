#pragma once

#include <chrono>

class Timer
{
protected:
	std::chrono::high_resolution_clock _clock;
	std::chrono::time_point<std::chrono::high_resolution_clock> _start;

public:
	Timer();
	~Timer();

	void Start();
	double GetElapsedTime();
};

