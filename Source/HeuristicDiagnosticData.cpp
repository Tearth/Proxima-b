#include "Headers/HeuristicDiagnosticData.h"

HeuristicDiagnosticData::HeuristicDiagnosticData()
{
	MaterialBalance = 0;
	KingSecurityBalance = 0;
	KingPositionBalance = 0;
	DoubledPawnsBalance = 0;
	BoardControlBalance = 0;
	PawnsPositionBalance = 0;

	GamePhase = EGamePhase::None;
}

HeuristicDiagnosticData::~HeuristicDiagnosticData()
{

}