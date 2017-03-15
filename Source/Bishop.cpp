#include "Headers/Bishop.h"

Bishop::Bishop()
{
	_name = "Bishop";
	_id = 'b';
}

Bishop::~Bishop()
{

}

vector<Move> Bishop::GetAvailableMoves(Board board, Position pos, EColor color)
{
	vector<Move> availableMoves;

	availableMoves = mergeMoves(availableMoves, getMovesAtSide(pos, board, 1, 1));
	availableMoves = mergeMoves(availableMoves, getMovesAtSide(pos, board, -1, -1));
	availableMoves = mergeMoves(availableMoves, getMovesAtSide(pos, board, 1, -1));
	availableMoves = mergeMoves(availableMoves, getMovesAtSide(pos, board, -1, 1));

	return availableMoves;
}

string Bishop::GetSpriteName(EColor color)
{
	if (color == EColor::White) return "Bishop white";
	else						return "Bishop black";
}