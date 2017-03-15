#pragma once
#pragma once

#include "Headers/PieceBase.h"
#include "Headers/Enums.h"

class Rook : public PieceBase
{
public:
	Rook();
	~Rook();

	string GetSpriteName(EColor color);
	vector<Move> GetAvailableMoves(Board board, Position pos, EColor color);

private:
};