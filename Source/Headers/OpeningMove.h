#pragma once
#include "Headers/Move.h"

class OpeningMove
{
public:
	OpeningMove();
	OpeningMove(unsigned long long int hash, Move move);
	~OpeningMove();

	unsigned long long int BoardHash;
	Move CurrentMove;
};