#pragma once
#include "Headers/Move.h"

class SortedMove
{
public:
	SortedMove();
	~SortedMove();

	Move CurrentMove;
	int Value;
};