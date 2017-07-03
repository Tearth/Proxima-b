#include "Headers/AI.h"

AI::AI()
{
	useOpeningsDB = true;
	MaxTime = 3.0f;
}

AI::~AI()
{

}

void AI::Init(Console* console)
{
	_console = console;
	_openingsDB.LoadDB();
}

Move AI::GetBestMove(Board board, EColor color)
{
	if(useOpeningsDB)
	{
		unsigned long long int hash = board.GetHash();
		vector<Opening> availableOpenings = _openingsDB.GetPossibleOpenings(hash, board.MovesCount);

		if (availableOpenings.size() == 0)
		{
			useOpeningsDB = false;
		}
		else
		{
			int openingIndex = availableOpenings.size() == 1 ? 0 : Random::Rand(0, availableOpenings.size() - 1);

			_console->AddNewLine("Move from opening book");
			_console->AddNewLine(availableOpenings[openingIndex].Name);

			return availableOpenings[openingIndex].Moves[board.MovesCount].CurrentMove;
		}
	}

	MinMaxDiagnosticData d;

	MinMaxInfo minMaxInfo;
	minMaxInfo.Board = board;
	minMaxInfo.CurrentColor = color;
	minMaxInfo.Move = Move(Position(-1, -1), Position(-1, -1));

	_transpositions.Clear();

	MinMaxResult result;
	float time = 0;
	int i = 0;

	Timer::Start("MinMaxTime");

	while(time <= MaxTime)
	{
		i++;
		minMaxInfo.CurrentDepth = i;
		minMaxInfo.InitialDepth = i;
		result = minMax(minMaxInfo, BLACK_MATE_SCORE, WHITE_MATE_SCORE, d);

		time = Timer::ElapsedTime("MinMaxTime");
	}

	time = Timer::Stop("MinMaxTime");

	if(color == EColor::White)
		_lastWhitePVNodes = getPVNodes(board);
	else
		_lastBlackPVNodes = getPVNodes(board);

	_console->AddNewLine("Total nodes: " + to_string(d.TotalNodesCount));
	_console->AddNewLine("End nodes: " + to_string(d.EndNodesCount));
	_console->AddNewLine("Max depth: " + to_string(d.MaxDepth));
	_console->AddNewLine("Alpha beta cut: " + to_string(d.AlphaBetaCut));
	_console->AddNewLine("Transposition added: " + to_string(d.TranspositionsAdded));
	_console->AddNewLine("Transpositions skipped: " + to_string(d.TranspositionsSkipped));
	_console->AddNewLine("");
	_console->AddNewLine("Q Total nodes: " + to_string(d.QuiescenceTotalNodes));
	_console->AddNewLine("Q Max depth: " + to_string(d.QuiescenceMaxNode));
	_console->AddNewLine("Q Transpositions added: " + to_string(d.QuiescenceTranspositionsAdded));
	_console->AddNewLine("Q Transpositions skipped: " + to_string(d.QuiescenceTranspositionsSkipped));
	_console->AddNewLine("");
	_console->AddNewLine("Best move: " + result.BestMove.From.ToString() + " -> " + result.BestMove.To.ToString());
	_console->AddNewLine("Time: " + to_string(time));
	_console->AddNewLine("Loops: " + to_string(i));

	return result.BestMove;
}

vector<Move> AI::GetAllBoardMoves(Board board, EColor currentColor)
{
	vector<Move> moves;
	for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 8; y++)
		{
			Position currentPos = Position(x, y);
			char field = board.GetFieldAtPosition(Position(x, y));

			if (field != '0' && board.GetColorByPieceID(field) == currentColor)
			{
				PieceBase* piece = PiecesManager::GetPieceByID(field);
				if (piece->GetID() == 'p')
					dynamic_cast<Pawn*>(piece)->SetEnPassantMoves(board.EnPassantPositions);

				vector<Move> availableMoves = piece->GetAvailableMoves(board, currentPos, currentColor);

				for (int z = 0; z < availableMoves.size(); z++)
				{
					char fieldTo = board.GetFieldAtPosition(availableMoves[z].To);
					if (fieldTo != 'k' && fieldTo != 'K')
						moves.push_back(availableMoves[z]);
				}
			}
		}
	}

	return moves;
}

MinMaxResult AI::minMax(MinMaxInfo minMaxInfo, int alpha, int beta, MinMaxDiagnosticData &d)
{
	EColor opponentColor = inverseColor(minMaxInfo.CurrentColor);
	int max = minMaxInfo.CurrentColor == EColor::White ? 1 : -1;

	unsigned long long int boardHash = minMaxInfo.Board.GetHash();
	int originalAlpha = alpha;

	if (_transpositions.Exist(boardHash))
	{
		TableItem item = _transpositions.Get(boardHash);
		if (item.Depth >= minMaxInfo.CurrentDepth)
		{
			d.TranspositionsSkipped++;

			if (item.Type == ValueType::Exact)
				return MinMaxResult(item.Value, item.Depth);
			else if (item.Type == ValueType::Lower)
			{
				if (item.Value > alpha)
					alpha = item.Value;
			}
			else if (item.Type == ValueType::Upper)
			{
				if (item.Value < beta)
					beta = item.Value;
			}

			if (alpha >= beta)
				return MinMaxResult(item.Value, item.Depth);
		}
	}

	if (IsMate(minMaxInfo.Board, EColor::White) || IsMate(minMaxInfo.Board, EColor::Black))
	{
		int value = max;
		if (minMaxInfo.CurrentColor == EColor::White)
			value *= BLACK_MATE_SCORE;
		else
			value *= WHITE_MATE_SCORE;

		d.EndNodesCount++;
		return MinMaxResult(value, minMaxInfo.CurrentDepth);
	}

	if (minMaxInfo.CurrentDepth <= 0)
	{
		MinMaxInfo info;
		info.Board = minMaxInfo.Board;
		info.CurrentColor = minMaxInfo.CurrentColor;
		info.InitialDepth = minMaxInfo.InitialDepth;
		info.CurrentDepth = minMaxInfo.CurrentDepth;
		
		MinMaxResult result = quiescenceSearch(info, alpha, beta, d);

		d.EndNodesCount++;
		return result;
	}

	vector<Move> moves = GetAllBoardMoves(minMaxInfo.Board, minMaxInfo.CurrentColor);
	moves = sortMoves(moves, minMaxInfo.Board, minMaxInfo.CurrentColor);

	vector<MinMaxResult> results;
	MinMaxResult best(INT32_MIN, 0);

	for (int i = 0; i < moves.size(); i++)
	{
		Board boardAfterMove = minMaxInfo.Board;
		Move move = moves[i];

		boardAfterMove.DoMove(move);

		if ((minMaxInfo.CurrentColor == EColor::White && IsCheck(boardAfterMove, EColor::White)) ||
			(minMaxInfo.CurrentColor == EColor::Black && IsCheck(boardAfterMove, EColor::Black)))
			continue;

		MinMaxInfo info;
		info.Board = boardAfterMove;
		info.CurrentColor = opponentColor;
		info.InitialDepth = minMaxInfo.InitialDepth;
		info.CurrentDepth = minMaxInfo.CurrentDepth - 1;

		MinMaxResult possibleBest = minMax(info, -beta, -alpha, d);
		possibleBest.Value = -possibleBest.Value;
		possibleBest.BestMove = moves[i];

		results.push_back(possibleBest);

		if (possibleBest.Value > best.Value)
			best = possibleBest;

		if (possibleBest.Value > alpha)
			alpha = possibleBest.Value;

		if (alpha >= beta)
		{
			d.AlphaBetaCut++;
			break;
		}
	}

	TableItem item(ValueType::None, best.Value, best.Depth, best.BestMove);
	if (best.Value <= originalAlpha)
		item.Type = ValueType::Upper;
	else if (best.Value >= beta)
		item.Type = ValueType::Lower;
	else
		item.Type = ValueType::Exact;
	_transpositions.Add(boardHash, item);
	d.TranspositionsAdded++;

	if (minMaxInfo.CurrentDepth == minMaxInfo.InitialDepth)
		drawDiagInfo(results, best, minMaxInfo.Board);

	d.TotalNodesCount++;

	return best;
}

MinMaxResult AI::quiescenceSearch(MinMaxInfo minMaxInfo, int alpha, int beta, MinMaxDiagnosticData &d)
{
	EColor opponentColor = inverseColor(minMaxInfo.CurrentColor);
	int max = minMaxInfo.CurrentColor == EColor::White ? 1 : -1;
	int originalAlpha = alpha;

	unsigned long long int boardHash = minMaxInfo.Board.GetHash();
	d.QuiescenceTotalNodes++;

	if (d.QuiescenceMaxNode > minMaxInfo.CurrentDepth)
		d.QuiescenceMaxNode = minMaxInfo.CurrentDepth;

	HeuristicDiagnosticData diag;
	int boardValue = max * _heuristic.GetBoardValue(minMaxInfo.Board, diag);

	if (boardValue >= beta)
		return MinMaxResult(beta, minMaxInfo.CurrentDepth);

	if (alpha < boardValue)
		alpha = boardValue;

	if (_transpositions.Exist(boardHash))
	{
		TableItem item = _transpositions.Get(boardHash);
		d.QuiescenceTranspositionsSkipped++;

		if (item.Type == ValueType::Exact)
			return MinMaxResult(item.Value, item.Depth);
		else if (item.Type == ValueType::Lower)
		{
			if (item.Value > alpha)
				alpha = item.Value;
		}
		else if (item.Type == ValueType::Upper)
		{
			if (item.Value < beta)
				beta = item.Value;
		}

		if (alpha >= beta)
			return MinMaxResult(item.Value, item.Depth);
	}

	vector<Move> moves = GetAllBoardMoves(minMaxInfo.Board, minMaxInfo.CurrentColor);
	moves = quiescenceSortMoves(moves, minMaxInfo.Board);

	MinMaxResult best(INT32_MIN, 0);
	for (int i = 0; i < moves.size(); i++)
	{
		Board boardAfterMove = minMaxInfo.Board;
		Move move = moves[i];

		char fieldTo = boardAfterMove.GetFieldAtPosition(moves[i].To);
		if (fieldTo == '0')
			continue;

		boardAfterMove.DoMove(move);
		
		if ((minMaxInfo.CurrentColor == EColor::White && IsCheck(boardAfterMove, EColor::White)) ||
			(minMaxInfo.CurrentColor == EColor::Black && IsCheck(boardAfterMove, EColor::Black)))
			continue;

		MinMaxInfo info;
		info.Board = boardAfterMove;
		info.CurrentColor = opponentColor;
		info.InitialDepth = minMaxInfo.InitialDepth;
		info.CurrentDepth = minMaxInfo.CurrentDepth - 1;

		MinMaxResult possibleBest = quiescenceSearch(info, -beta, -alpha, d);
		possibleBest.Value = -possibleBest.Value;
		possibleBest.BestMove = moves[i];

		if (possibleBest.Value >= beta)
			return MinMaxResult(beta, possibleBest.Depth);

		if (possibleBest.Value > alpha)
		{
			best = possibleBest;
			alpha = possibleBest.Value;
		}
	}

	if (best.Value != INT32_MIN)
	{
		TableItem item(ValueType::None, best.Value, best.Depth, best.BestMove);
		if (best.Value <= originalAlpha)
			item.Type = ValueType::Upper;
		else if (best.Value >= beta)
			item.Type = ValueType::Lower;
		else
			item.Type = ValueType::Exact;
		_transpositions.Add(boardHash, item);

		d.QuiescenceTranspositionsAdded++;
	}

	int tmpDepth;
	if (best.Value == INT32_MIN)
		tmpDepth = minMaxInfo.CurrentDepth;
	else
		tmpDepth = best.Depth;
	return MinMaxResult(alpha, tmpDepth);
}

vector<Move> AI::sortMoves(vector<Move> moves, Board board, EColor currentColor)
{
	unsigned long long int boardHash = board.GetHash();
	bool bestFinded = false;
	int i = 0;

	Move bestMove;
	if (_transpositions.Exist(boardHash))
	{
		TableItem t = _transpositions.Get(boardHash);
		if (t.BestMove.From != Position(-1, -1) && t.BestMove.To != Position(-1, -1))
		{
			bestMove = t.BestMove;
			bestFinded = true;
		}
	}
		
	vector<Move> parsedMoves;
	for (int x = 0; x < moves.size(); x++)
	{
		Move moveToChange = moves[x];

		if ((bestFinded && moves[x].From == bestMove.From && moves[x].To == bestMove.To))
		{
			parsedMoves.insert(parsedMoves.begin(), moveToChange);
			i++;
		}
		else
		{
			if (board.GetFieldAtPosition(moves[x].To) != '0' || moves[x].IsPromotion)
			{
				parsedMoves.insert(parsedMoves.begin() + i, moveToChange);
				i++;
			}
			else
			{
				parsedMoves.push_back(moves[x]);
			}
		}
	}
		
	return parsedMoves;
}

vector<Move> AI::quiescenceSortMoves(vector<Move> moves, Board board)
{
	unsigned long long int boardHash = board.GetHash();

	float bestRatio = -999999;
	float worstRatio = 999999;

	vector<Move> listOfCaptures;
	for (int i = 0; i < moves.size(); i++)
	{
		char pieceFrom = board.GetFieldAtPosition(moves[i].From);
		char pieceTo = board.GetFieldAtPosition(moves[i].To);

		if (pieceTo == '0')
			continue;

		int pieceFromValue = abs(_heuristic.GetPieceValue(pieceFrom));
		int pieceToValue = abs(_heuristic.GetPieceValue(pieceTo));

		float ratio = pieceToValue / pieceFromValue;
		if (ratio >= bestRatio)
		{
			bestRatio = ratio;
			listOfCaptures.insert(listOfCaptures.begin(), moves[i]);
		}
		else if (ratio <= worstRatio)
		{
			worstRatio = ratio;
			listOfCaptures.push_back(moves[i]);
		}
		else
		{
			listOfCaptures.insert(listOfCaptures.begin() + (listOfCaptures.size() / 2), moves[i]);
		}
	}

	return listOfCaptures;
}

void AI::drawDiagInfo(vector<MinMaxResult> results, MinMaxResult best, Board board)
{
	MinMaxResult x;
	for (int i = 0; i < results.size(); i++)
	{
		_console->AddNewLine(results[i].BestMove.From.ToString() +
			" -> " + results[i].BestMove.To.ToString() +
			"    d:" + to_string(results[i].Depth) +
			"    v:" + to_string(results[i].Value));
	}

	Board diagBoard = board;

	_console->AddNewLine("");
	_console->AddNewLine("Sequence:");

	vector<PVNode> pvNodes = getPVNodes(board);
	for(int i=0; i<pvNodes.size(); i++)
	{
		_console->AddNewLine(pvNodes[i].BestMove.From.ToString() + " -> " + pvNodes[i].BestMove.To.ToString());
	}

	_console->AddNewLine("");
}

vector<PVNode> AI::getPVNodes(Board board)
{
	Board diagBoard = board;
	vector<PVNode> pvNodes;

	int count = 0;
	while (true)
	{
		if (count > 10)
			break;

		unsigned long long int hash = diagBoard.GetHash();

		if (!_transpositions.Exist(hash))
			break;

		Move m = _transpositions.Get(hash).BestMove;
		pvNodes.push_back(PVNode(hash, m));

		diagBoard.DoMove(m);
		count++;
	}

	return pvNodes;
}

MinMaxResult AI::getBestMinMaxResult(vector<MinMaxResult> results, bool max)
{
	MinMaxResult best;
	bool init = true;

	for (int i = 0; i < results.size(); i++)
	{
		if (init)
		{
			best = results[0];
			init = false;
			continue;
		}

		if (max)
		{
			if (results[i].Value > best.Value)
				best = results[i];
		}
		else
		{
			if (results[i].Value < best.Value)
				best = results[i];
		}
	}

	return best;
}

vector<Position> AI::GetFieldAttackers(Board board, Position pos, EColor enemyColor)
{
	return _heuristic.GetFieldAttackersHeuristic(board, pos, enemyColor);
}

vector<Move> AI::CheckKingMoves(Board board, vector<Move> moves, Position pos)
{
	char king = board.GetFieldAtPosition(pos);
	EColor kingColor = board.GetColorByPieceID(king);

	EColor opponentColor = inverseColor(kingColor);

	board.Set(pos, '0');
	int i = 0;
	while(i < moves.size())
	{
		vector<Position> attackers = GetFieldAttackers(board, moves[i].To, opponentColor);
		if (attackers.size() != 0)
			moves.erase(moves.begin() + i);
		else
			i++;
	}

	return moves;
}

bool AI::IsCheck(Board board, EColor kingColor)
{
	EColor opponentColor = inverseColor(kingColor);

	char kingID;
	if (kingColor == EColor::White)
		kingID = 'k';
	else
		kingID = 'K';

	Position kingPos = board.FindPiece(kingID)[0];
	if (GetFieldAttackers(board, kingPos, opponentColor).size() != 0)
		return true;
	else
		return false;
}

int AI::RunHeuristic(Board board)
{
	HeuristicDiagnosticData diagnosticData;
	int result = _heuristic.GetBoardValue(board, diagnosticData);

	_console->AddNewLine("Heuristic result: ");
	_console->AddNewLine("");
	_console->AddNewLine("WHITE     ");
	_console->AddNewLine("    material: " + to_string(diagnosticData.MaterialBalance));
	_console->AddNewLine("    king security: " + to_string(diagnosticData.KingSecurityBalance));
	_console->AddNewLine("    king position: " + to_string(diagnosticData.KingPositionBalance));
	_console->AddNewLine("    pawns balance: " + to_string(diagnosticData.DoubledPawnsBalance));
	_console->AddNewLine("    controll balance: " + to_string(diagnosticData.BoardControlBalance));
	_console->AddNewLine("    pawns position: " + to_string(diagnosticData.PawnsPositionBalance));

	string gamePhaseString;
	if		(diagnosticData.GamePhase == EGamePhase::Opening) gamePhaseString = "opening";
	else if (diagnosticData.GamePhase == EGamePhase::Middlegame) gamePhaseString = "middlegame";
	else if (diagnosticData.GamePhase == EGamePhase::Endgame) gamePhaseString = "endgame";

	_console->AddNewLine("");
	_console->AddNewLine("    total rate: " + to_string(result));
	_console->AddNewLine("    game phase: " + gamePhaseString);

	return result;
}

bool AI::IsMate(Board board, EColor kingColor)
{
	if (!IsCheck(board, kingColor))
		return false;

	vector<Move> moves = GetAllBoardMoves(board, kingColor);
	bool mate = true;

	EColor opponentColor = inverseColor(kingColor);
	for (int i = 0; i < moves.size(); i++)
	{
		Board b = board;
		b.DoMove(moves[i]);

		Position kingPosition;
		if (kingColor == EColor::White)
			kingPosition = b.FindPiece('k')[0];
		else
			kingPosition = b.FindPiece('K')[0];

		if (GetFieldAttackers(b, kingPosition, opponentColor).size() == 0)
		{
			mate = false;
			break;
		}
	}

	return mate;
}

void AI::Reset()
{
	useOpeningsDB = true;
}