#pragma once
#pragma once

#include "Headers/PieceBase.h"
#include "Headers/Enums.h"
#include "Headers/ColorOperations.h"

class King : public PieceBase
{
public:
	King();
	~King();

	string GetSpriteName(EColor color);
	vector<Move> GetAvailableMoves(Board board, Position pos, EColor color);

private:
};