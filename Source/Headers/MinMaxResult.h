#pragma once
#include "Headers/Move.h"

class MinMaxResult
{
public:
	MinMaxResult();
	MinMaxResult(int value, int depth);
	~MinMaxResult();

	int Value;
	int Depth;
	Move BestMove;
};