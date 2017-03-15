#include "Headers/KingSafetyRate.h"

KingSafetyRate::KingSafetyRate()
{
	WhiteKingPositionRate = 0;
	WhitePiecesShieldRate = 0;
	WhiteDangerousRate = 0;

	BlackKingPositionRate = 0;
	BlackPiecesShieldRate = 0;
	BlackDangerousRate = 0;

	TotalRate = 0;
}

KingSafetyRate::~KingSafetyRate()
{

}