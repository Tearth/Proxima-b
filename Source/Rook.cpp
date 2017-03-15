#include "Headers/Rook.h"

Rook::Rook()
{
	_name = "Rook";
	_id = 'r';
}

Rook::~Rook()
{

}

vector<Move> Rook::GetAvailableMoves(Board board, Position pos, EColor color)
{
	vector<Move> availableMoves;

	availableMoves = mergeMoves(availableMoves, getMovesAtSide(pos, board, 1, 0));
	availableMoves = mergeMoves(availableMoves, getMovesAtSide(pos, board, -1, 0));
	availableMoves = mergeMoves(availableMoves, getMovesAtSide(pos, board, 0, 1));
	availableMoves = mergeMoves(availableMoves, getMovesAtSide(pos, board, 0, -1));

	return availableMoves;
}

string Rook::GetSpriteName(EColor color)
{
	if (color == EColor::White) return "Rook white";
	else						return "Rook black";
}