#pragma once
#include <string>
#include "Headers/Move.h"
#include "Headers/Enums.h"

using namespace std;

class TableItem
{
public:
	TableItem();
	TableItem(ValueType type, int value, int depth, Move bestMove);
	~TableItem();

	ValueType Type;
	int Value;
	int Depth;
	Move BestMove;
};