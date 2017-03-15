#include "Headers/OpeningMove.h"

OpeningMove::OpeningMove() : OpeningMove(0, Move())
{
	
}

OpeningMove::OpeningMove(unsigned long long int hash, Move move)
{
	BoardHash = hash;
	CurrentMove = move;
}

OpeningMove::~OpeningMove()
{

}