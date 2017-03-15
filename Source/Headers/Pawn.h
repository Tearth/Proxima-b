#pragma once

#include "Headers/PieceBase.h"
#include "Headers/Enums.h"

class Pawn : public PieceBase
{
public:
	Pawn();
	~Pawn();

	string GetSpriteName(EColor color);
	vector<Move> GetAvailableMoves(Board board, Position pos, EColor color);

	void SetEnPassantMoves(vector<Position> enPassantMoves);

private:
	vector<Move> checkPromotions(vector<Move> moves, EColor color);
	vector<Move> getListOfPromotions(Move move, EColor color);
	bool isEnPassant(Position pos);

	vector<Position> _enPassantMoves;
};