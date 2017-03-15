#pragma once

class MinMaxDiagnosticData
{
public:
	MinMaxDiagnosticData();
	~MinMaxDiagnosticData();

	int TotalNodesCount;
	int EndNodesCount;
	int MaxDepth;
	int AlphaBetaCut;
	int TranspositionsAdded;
	int TranspositionsSkipped;

	int QuiescenceTotalNodes;
	int QuiescenceMaxNode;
	int QuiescenceTranspositionsAdded;
	int QuiescenceTranspositionsSkipped;
};