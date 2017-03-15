#pragma once
#include "Headers/Board.h"
#include "Headers/HeuristicDiagnosticData.h"
#include "Headers/PieceBase.h"
#include "Headers/PiecesManager.h"
#include "Headers/Move.h"
#include "Headers/MaterialRateData.h"
#include "Headers/KingSafetyRate.h"
#include "Headers/MobilityRate.h"

class Heuristic
{
public:
	Heuristic();
	~Heuristic();

	//Returns the heuristic value of the specific board
	int GetBoardValue(Board board, HeuristicDiagnosticData &d);

	//Returns the value of specific piece
	int GetPieceValue(char field);

	//Returns the list of pieces which are attacking the specific field
	vector<Position> GetFieldAttackersHeuristic(Board board, Position pos, EColor enemyColor);

private:
	EGamePhase GetGamePhase(int whiteMaterial, int blackMaterial, Position whiteKing, Position blackKing);
	void ParseMaterialRate(Position pos, Board board, int &whiteMaterial, int &blackMaterial);
	void ParseKingSecurity(Position pos, Board board, int &whiteKingSecurity, int &blackKingSecurity);
	void ParseKingPosition(Position whiteKing, Position blackKing, EGamePhase phase, int &whiteKingPositionRate, int &blackKingPositionRate);
	void ParseDoubledPawns(Position pos, Board board, int (&doubledWhitePawns)[8], int(&doubledBlackPawns)[8]);
	void ParseBoardControl(Board board, EGamePhase phase, int &whiteBoardControl, int &blackBoardControl);
	void ParsePawns(Position pos, Board board, int &whitePawnsPosition, int &blackPawnsPosition);

	int kingMaterialRate;
};