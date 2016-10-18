#include "../include/Timer.h"

Timer::Timer()
{
	LARGE_INTEGER pcFreq;
	QueryPerformanceFrequency(&pcFreq);
	_frequency = 1.0f / (pcFreq.QuadPart);
}

Timer::~Timer()
{}

void Timer::Start()
{
	QueryPerformanceCounter(&_timerStart);
}

double Timer::GetElapsedTime()
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	return (li.QuadPart - _timerStart.QuadPart) * _frequency;
}
