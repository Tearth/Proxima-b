#pragma once
#include "Headers/Move.h"
#include "Headers/Opening.h"

class OpeningDBResult
{
public:
	OpeningDBResult();
	~OpeningDBResult();

	Move BestMove;
	Opening SelectedOpening;
};