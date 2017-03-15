#include "Headers/Random.h"


void Random::Init()
{
	srand(time(NULL));
}

int Random::Rand(int from, int to)
{
	return (rand() % to) + from + 1;
}