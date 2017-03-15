#pragma once

#include "Headers/PieceBase.h"
#include "Headers/Enums.h"

class Bishop : public PieceBase
{
public:
	Bishop();
	~Bishop();

	string GetSpriteName(EColor color);
	vector<Move> GetAvailableMoves(Board board, Position pos, EColor color);

private:
};