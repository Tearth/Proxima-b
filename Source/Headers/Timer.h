#pragma once
#include <string>
#include <vector>
#include "Headers/TimerRecord.h"

using namespace std;

static class Timer
{
public:
	static void Start(string name);
	static float ElapsedTime(string name);
	static float Stop(string name);

private:
	static vector<TimerRecord> _timers;
};