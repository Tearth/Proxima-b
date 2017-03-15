#pragma once
#include "Headers/Position.h"

class Move
{
public:
	Move();
	Move(const Move &m);
	Move(Position from, Position to);
	Move(Position from, Position to, bool isPromotion, char promotionPiece);
	~Move();

	Position From;
	Position To;

	bool IsPromotion;
	char PromotionPiece;
private:
};