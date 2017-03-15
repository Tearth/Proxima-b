#include "Headers/PieceBase.h"

string PieceBase::GetName()
{
	return _name;
}


char PieceBase::GetID()
{
	return _id;
}

vector<Move> PieceBase::getMovesAtSide(Position currentPos, Board board, int x, int y) 
{
	Position currentMove;
	vector<Move> availableMoves;

	for (int i = 1; i < 8; i++)
	{
		currentMove = currentPos + Position(x*i, y*i);
		if (board.IsPositionValid(currentMove))
		{
			bool canKill = board.CanKill(currentPos, currentMove);
			if (board.GetFieldAtPosition(currentMove) == '0' || canKill)
			{
				availableMoves.push_back(Move(currentPos, currentMove));
			}
			else
				break;

			if (canKill)
				break;
		}
	}

	return availableMoves;
}

vector<Move> PieceBase::mergeMoves(vector<Move> moves1, vector<Move> moves2)
{
	moves1.insert(moves1.end(), moves2.begin(), moves2.end());
	return moves1;
}