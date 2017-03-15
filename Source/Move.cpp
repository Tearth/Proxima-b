#include "Headers/Move.h"

Move::Move()
{
	IsPromotion = false;
}

Move::Move(const Move &m)
{
	From = m.From;
	To = m.To;
	IsPromotion = m.IsPromotion;
	PromotionPiece = m.PromotionPiece;
}

Move::Move(Position from, Position to) : Move(from, to, false, '0')
{
	
}

Move::Move(Position from, Position to, bool isPromotion, char promotionPiece)
{
	From = from;
	To = to;
	IsPromotion = isPromotion;
	PromotionPiece = promotionPiece;
}

Move::~Move()
{

}