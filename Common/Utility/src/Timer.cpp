#include "../include/Timer.h"

Timer::Timer()
{
}

Timer::~Timer()
{}

void Timer::Start()
{
	_start = _clock.now();
}

double Timer::GetElapsedTime()
{
	return std::chrono::duration_cast<std::chrono::duration<double>>(_clock.now() - _start).count();
}
