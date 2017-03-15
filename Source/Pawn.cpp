#include "Headers/Pawn.h"

Pawn::Pawn()
{
	_name = "Pawn";
	_id = 'p';
}

Pawn::~Pawn()
{

}

vector<Move> Pawn::GetAvailableMoves(Board board, Position pos, EColor color)
{
	vector<Move> availableMoves;
	char dir = color == EColor::White ? -1 : 1;
	char startLine = color == EColor::White ? 6 : 1;

	Position currentMove;

	currentMove = pos + (Position(0, 1) * dir);
	if (board.IsPositionValid(currentMove) && board.GetFieldAtPosition(currentMove) == '0')
		availableMoves.push_back(Move(pos, currentMove));

	if (pos.Y == startLine && availableMoves.size() > 0)
	{
		currentMove = pos + (Position(0, 2) * dir);
		if (board.IsPositionValid(currentMove) && board.GetFieldAtPosition(currentMove) == '0')
			availableMoves.push_back(Move(pos, currentMove));
	}

	currentMove = pos + (Position(0, 1) * dir) + Position(1, 0);
	if((board.IsPositionValid(currentMove) && board.GetFieldAtPosition(currentMove) != '0' && board.CanKill(pos, currentMove))
		|| isEnPassant(currentMove))
		availableMoves.push_back(Move(pos, currentMove));

	currentMove = pos + (Position(0, 1) * dir) - Position(1, 0);
	if ((board.IsPositionValid(currentMove) && board.GetFieldAtPosition(currentMove) != '0' && board.CanKill(pos, currentMove))
		|| isEnPassant(currentMove))
		availableMoves.push_back(Move(pos, currentMove));

	availableMoves = checkPromotions(availableMoves, color);
	return availableMoves;
}

string Pawn::GetSpriteName(EColor color)
{
	if (color == EColor::White) return "Pawn white";
	else						return "Pawn black";
}

vector<Move> Pawn::checkPromotions(vector<Move> moves, EColor color)
{
	vector<Move> parsedMoves;
	for (int i = 0; i < moves.size(); i++)
	{
		if (color == EColor::White && moves[i].To.Y == 0)
		{
			vector<Move> promotions = getListOfPromotions(moves[i], EColor::White);
			parsedMoves.insert(parsedMoves.end(), promotions.begin(), promotions.end());
		}
		else if (color == EColor::Black && moves[i].To.Y == 7)
		{
			vector<Move> promotions = getListOfPromotions(moves[i], EColor::Black);
			parsedMoves.insert(parsedMoves.end(), promotions.begin(), promotions.end());
		}
		else
		{
			parsedMoves.push_back(moves[i]);
		}
	}

	return parsedMoves;
}

vector<Move> Pawn::getListOfPromotions(Move move, EColor color)
{
	vector<Move> moves;

	if (color == EColor::White)
	{
		moves.push_back(Move(move.From, move.To, true, 'q'));
		moves.push_back(Move(move.From, move.To, true, 'r'));
		moves.push_back(Move(move.From, move.To, true, 'b'));
		moves.push_back(Move(move.From, move.To, true, 'n'));
	}
	else
	{
		moves.push_back(Move(move.From, move.To, true, 'Q'));
		moves.push_back(Move(move.From, move.To, true, 'R'));
		moves.push_back(Move(move.From, move.To, true, 'B'));
		moves.push_back(Move(move.From, move.To, true, 'N'));
	}
	
	return moves;
}

void Pawn::SetEnPassantMoves(vector<Position> enPassantMoves)
{
	_enPassantMoves = enPassantMoves;
}

bool Pawn::isEnPassant(Position pos)
{
	for (int i = 0; i < _enPassantMoves.size(); i++)
	{
		if (_enPassantMoves[i] == pos)
			return true;
	}

	return false;
}