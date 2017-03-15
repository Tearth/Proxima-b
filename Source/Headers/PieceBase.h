#pragma once
#include <vector>
#include <string>
#include "Headers/Board.h"
#include "Headers/Move.h"
#include "Headers/Enums.h"

using namespace std;

class PieceBase
{
public:
	string GetName();
	char GetID();

	virtual string GetSpriteName(EColor color) = 0;
	virtual vector<Move> GetAvailableMoves(Board board, Position pos, EColor color) = 0;

protected:
	vector<Move> getMovesAtSide(Position currentPos, Board board, int x, int y);
	vector<Move> mergeMoves(vector<Move> moves1, vector<Move> moves2);

	string _name;
	char _id;
};