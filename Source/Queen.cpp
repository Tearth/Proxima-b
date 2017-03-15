#include "Headers/Queen.h"

Queen::Queen()
{
	_name = "Queen";
	_id = 'q';
}

Queen::~Queen()
{

}

vector<Move> Queen::GetAvailableMoves(Board board, Position pos, EColor color)
{
	vector<Move> availableMoves;

	availableMoves = mergeMoves(availableMoves, getMovesAtSide(pos, board, 1, 0));
	availableMoves = mergeMoves(availableMoves, getMovesAtSide(pos, board, -1, 0));
	availableMoves = mergeMoves(availableMoves, getMovesAtSide(pos, board, 0, 1));
	availableMoves = mergeMoves(availableMoves, getMovesAtSide(pos, board, 0, -1));

	availableMoves = mergeMoves(availableMoves, getMovesAtSide(pos, board, 1, 1));
	availableMoves = mergeMoves(availableMoves, getMovesAtSide(pos, board, -1, -1));
	availableMoves = mergeMoves(availableMoves, getMovesAtSide(pos, board, 1, -1));
	availableMoves = mergeMoves(availableMoves, getMovesAtSide(pos, board, -1, 1));

	return availableMoves;
}

string Queen::GetSpriteName(EColor color)
{
	if (color == EColor::White) return "Queen white";
	else						return "Queen black";
}