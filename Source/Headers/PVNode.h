#pragma once
#include "Headers/Move.h"

class PVNode
{
public:
	PVNode();
	PVNode(unsigned long long int hash, Move move);
	~PVNode();

	unsigned long long int BoardHash;
	Move BestMove;
};