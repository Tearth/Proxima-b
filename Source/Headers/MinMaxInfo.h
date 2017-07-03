#pragma once
#include "Headers/Board.h"
#include "Headers/Move.h"

class MinMaxInfo
{
public:
	MinMaxInfo();
	~MinMaxInfo();

	Board Board;
	EColor CurrentColor;

	int InitialDepth;
	int CurrentDepth;

	Move Move;
};