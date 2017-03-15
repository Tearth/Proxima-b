#pragma once
#include "Headers/MaterialRateData.h"
#include "Headers/Enums.h"
#include "Headers/KingSafetyRate.h"
#include "Headers/MobilityRate.h"

class HeuristicDiagnosticData
{
public:
	HeuristicDiagnosticData();
	~HeuristicDiagnosticData();

	int MaterialBalance;
	int KingSecurityBalance;
	int KingPositionBalance;
	int DoubledPawnsBalance;
	int BoardControlBalance;
	int PawnsPositionBalance;

	EGamePhase GamePhase;
};