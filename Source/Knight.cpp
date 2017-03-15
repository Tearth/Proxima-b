#include "Headers/Knight.h"

Knight::Knight()
{
	_name = "Knight";
	_id = 'n';
}

Knight::~Knight()
{

}

vector<Move> Knight::GetAvailableMoves(Board board, Position pos, EColor color)
{
	vector<Move> availableMoves;
	vector<Position> positionsToParse = { Position(1, 2),
										  Position(-1, 2),
										  Position(1, -2),
									  	  Position(-1, -2),
										  Position(2, 1),
										  Position(2, -1),
										  Position(-2, 1),
										  Position(-2, -1) };

	for (int i = 0; i < positionsToParse.size(); i++)
	{
		Position currentPosition = pos + positionsToParse[i];

		if (board.IsPositionValid(currentPosition) &&
			(board.GetFieldAtPosition(currentPosition) == '0' || board.CanKill(pos, currentPosition)))
			availableMoves.push_back(Move(pos, currentPosition));
	}

	return availableMoves;
}

string Knight::GetSpriteName(EColor color)
{
	if (color == EColor::White) return "Knight white";
	else						return "Knight black";
}