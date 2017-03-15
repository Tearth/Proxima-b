#pragma once
#include <time.h>
#include <random>

static class Random
{
public:
	static void Init();
	static int Rand(int from, int to);
};