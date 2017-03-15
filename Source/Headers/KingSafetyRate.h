#pragma once

class KingSafetyRate
{
public:
	KingSafetyRate();
	~KingSafetyRate();

	int WhiteKingPositionRate;
	int WhitePiecesShieldRate;
	int WhiteDangerousRate;

	int BlackKingPositionRate;
	int BlackPiecesShieldRate;
	int BlackDangerousRate;

	int TotalRate;
};