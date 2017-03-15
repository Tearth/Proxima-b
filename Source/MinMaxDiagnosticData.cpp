#include "Headers/MinMaxDiagnosticData.h"

MinMaxDiagnosticData::MinMaxDiagnosticData()
{
	TotalNodesCount = 0;
	EndNodesCount = 0;
	MaxDepth = 0;
	AlphaBetaCut = 0;
	TranspositionsAdded = 0;
	TranspositionsSkipped = 0;

	QuiescenceMaxNode = 0;
	QuiescenceTotalNodes = 0;
	QuiescenceTranspositionsAdded = 0;
	QuiescenceTranspositionsSkipped = 0;
}

MinMaxDiagnosticData::~MinMaxDiagnosticData()
{

}