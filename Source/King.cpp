#include "Headers/King.h"

King::King()
{
	_name = "King";
	_id = 'k';
}

King::~King()
{

}

vector<Move> King::GetAvailableMoves(Board board, Position pos, EColor color)
{
	vector<Move> availableMoves;

	for (int x = -1; x <= 1; x++)
	{
		for (int y = -1; y <= 1; y++)
		{
			if (x == 0 && y == 0)
				continue;

			Position currentPosition = pos + Position(x, y);

			if (board.IsPositionValid(currentPosition)  &&
				(board.GetFieldAtPosition(currentPosition) == '0' || board.CanKill(pos, currentPosition)))
			{
				availableMoves.push_back(Move(pos, currentPosition));
			}
		}
	}

	if (color == EColor::White && pos == Position(4, 7))
	{
		if (board.CastlingWhiteShort &&						   board.GetFieldAttackers(Position(4, 7), EColor::Black).size() == 0 &&
			board.GetFieldAtPosition(Position(5, 7)) == '0' && board.GetFieldAttackers(Position(5, 7), EColor::Black).size() == 0 &&
			board.GetFieldAtPosition(Position(6, 7)) == '0' && board.GetFieldAttackers(Position(6, 7), EColor::Black).size() == 0 &&
			board.GetFieldAtPosition(Position(7, 7)) == 'r')
			availableMoves.push_back(Move(pos, Position(6, 7)));

		if (board.CastlingWhiteLong &&						   board.GetFieldAttackers(Position(4, 7), EColor::Black).size() == 0 &&
			board.GetFieldAtPosition(Position(3, 7)) == '0' && board.GetFieldAttackers(Position(3, 7), EColor::Black).size() == 0 &&
			board.GetFieldAtPosition(Position(2, 7)) == '0' && board.GetFieldAttackers(Position(2, 7), EColor::Black).size() == 0 &&
			board.GetFieldAtPosition(Position(1, 7)) == '0' && board.GetFieldAttackers(Position(1, 7), EColor::Black).size() == 0 &&
			board.GetFieldAtPosition(Position(0, 7)) == 'r')
			availableMoves.push_back(Move(pos, Position(2, 7)));
	}
	
	if (color == EColor::Black && pos == Position(4, 0))
	{
		if (board.CastlingBlackShort &&						   board.GetFieldAttackers(Position(4, 0), EColor::White).size() == 0 &&
			board.GetFieldAtPosition(Position(5, 0)) == '0' && board.GetFieldAttackers(Position(5, 0), EColor::White).size() == 0 &&
			board.GetFieldAtPosition(Position(6, 0)) == '0' && board.GetFieldAttackers(Position(6, 0), EColor::White).size() == 0 &&
			board.GetFieldAtPosition(Position(7, 0)) == 'R')
			availableMoves.push_back(Move(pos, Position(6, 0)));

		if (board.CastlingBlackLong &&						   board.GetFieldAttackers(Position(4, 0), EColor::White).size() == 0 &&
			board.GetFieldAtPosition(Position(3, 0)) == '0' && board.GetFieldAttackers(Position(3, 0), EColor::White).size() == 0 &&
			board.GetFieldAtPosition(Position(2, 0)) == '0' && board.GetFieldAttackers(Position(2, 0), EColor::White).size() == 0 &&
			board.GetFieldAtPosition(Position(1, 0)) == '0' && board.GetFieldAttackers(Position(1, 0), EColor::White).size() == 0 &&
			board.GetFieldAtPosition(Position(0, 0)) == 'R')
			availableMoves.push_back(Move(pos, Position(2, 0)));
	}

	return availableMoves;
}

string King::GetSpriteName(EColor color)
{
	if (color == EColor::White) return "King white";
	else						return "King black";
}