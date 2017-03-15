#pragma once
#pragma once

#include "Headers/PieceBase.h"
#include "Headers/Enums.h"

class Knight : public PieceBase
{
public:
	Knight();
	~Knight();

	string GetSpriteName(EColor color);
	vector<Move> GetAvailableMoves(Board board, Position pos, EColor color);

private:
};