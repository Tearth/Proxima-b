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
#include "Headers/PVNode.h"

class AI
{
public:
	AI();
	~AI();

	//Inits AI
	void Init(Console* console);

	//Runs AI alghoritms for the specific board and returns the best move
	Move GetBestMove(Board board, EColor color);

	//Gets all possible moves for the specific board
	vector<Move> GetAllBoardMoves(Board board, EColor currentColor);

	//Gets the list of pieces positions which are attacking specific field
	vector<Position> GetFieldAttackers(Board board, Position pos, EColor enemyColor);

	//Gets the list of available moves for the specific king
	vector<Move> CheckKingMoves(Board board, vector<Move> moves, Position pos);

	//Returns the heurictic value of the specific board
	int RunHeuristic(Board board);

	//Returns true in king with specific color is mated
	bool IsMate(Board board, EColor kingColor);

	//Returns true if king with specific color is checked
	bool IsCheck(Board board, EColor kingColor);

	//Resets AI state
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