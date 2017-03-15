#pragma once
#include <limits>
#include <random>

#include "Headers/Board.h"
#include "Headers/Console.h"
#include "Headers/Move.h"
#include "Headers/PiecesManager.h"
#include "Headers/MinMaxDiagnosticData.h"
#include "Headers/MinMaxResult.h"
#include "Headers/Heuristic.h"
#include "Headers/ColorOperations.h"
#include "Headers/Timer.h"
#include "Headers/MinMaxInfo.h"
#include "Headers/HashTable.h"
#include "Headers/SortedMove.h"
#include "Headers/Defines.h"
#include "Headers/OpeningsDatabase.h"
#include "Headers/Random.h"

class PVNode
{
public:
	PVNode(unsigned long long int hash, Move move) { BoardHash = hash, BestMove = move; }
	unsigned long long int BoardHash;
	Move BestMove;
};

class AI
{
public:
	AI();
	~AI();

	void Init(Console* console);
	Move GetBestMove(Board board, EColor color);
	vector<Move> GetAllBoardMoves(Board board, EColor currentColor);

	vector<Position> GetFieldAttackers(Board board, Position pos, EColor enemyColor);
	vector<Move> CheckKingMoves(Board board, vector<Move> moves, Position pos);
	int RunHeuristic(Board board);
	bool IsMate(Board board, EColor kingColor);
	bool IsCheck(Board board, EColor kingColor);
	void Reset();

	float MaxTime;
private:
	MinMaxResult minMax(MinMaxInfo minMaxInfo, int alpha, int beta, MinMaxDiagnosticData &d);
	MinMaxResult quiescenceSearch(MinMaxInfo minMaxInfo, int alpha, int beta, MinMaxDiagnosticData &d);

	MinMaxResult getBestMinMaxResult(vector<MinMaxResult> results, bool max);
	void drawDiagInfo(vector<MinMaxResult> results, MinMaxResult best, Board board);
	vector<Move> sortMoves(vector<Move> moves, Board board, EColor currentColor);
	vector<Move> quiescenceSortMoves(vector<Move> moves, Board board);
	vector<PVNode> getPVNodes(Board board);
	
	Console* _console;
	Heuristic _heuristic;
	HashTable _transpositions;
	OpeningDatabase _openingsDB;
	vector<PVNode> _lastWhitePVNodes;
	vector<PVNode> _lastBlackPVNodes;

	bool useOpeningsDB;
};