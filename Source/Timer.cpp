#include "Headers/Timer.h"

vector<TimerRecord> Timer::_timers;

void Timer::Start(string name)
{
	TimerRecord timer(name);
	timer.Clock.restart();

	_timers.push_back(timer);
}

float Timer::ElapsedTime(string name)
{
	float result = -1;
	for (int i = 0; i < _timers.size(); i++)
	{
		if (_timers[i].Name == name)
		{
			result = _timers[i].Clock.getElapsedTime().asSeconds();
			break;
		}
	}

	return result;
}

float Timer::Stop(string name)
{
	float result = -1;
	for (int i = 0; i < _timers.size(); i++)
	{
		if (_timers[i].Name == name)
		{
			result = _timers[i].Clock.restart().asSeconds();
			_timers.erase(_timers.begin() + i);
			break;
		}
	}

	return result;
}