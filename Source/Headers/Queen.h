#pragma once
#pragma once

#include "Headers/PieceBase.h"
#include "Headers/Enums.h"

class Queen : public PieceBase
{
public:
	Queen();
	~Queen();

	string GetSpriteName(EColor color);
	vector<Move> GetAvailableMoves(Board board, Position pos, EColor color);

private:
};