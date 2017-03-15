#include "Headers/Heuristic.h"

Heuristic::Heuristic()
{
	kingMaterialRate = 1000000;
}

Heuristic::~Heuristic()
{

}

int Heuristic::GetBoardValue(Board board, HeuristicDiagnosticData &d)
{
	int whiteMaterial = 0;
	int blackMaterial = 0;

	int whiteKingSecurity = 0;
	int blackKingSecurity = 0;

	int whiteKingPositionRate = 0;
	int blackKingPositionRate = 0;

	int doubledWhitePawnsCount[8];
	memset(doubledWhitePawnsCount, 0, sizeof(int) * 8);

	int doubledBlackPawnsCount[8];
	memset(doubledBlackPawnsCount, 0, sizeof(int) * 8);

	int doubledWhitePawns = 0;
	int doubledBlackPawns = 0;

	int whiteBoardControl = 0;
	int blackBoardControl = 0;

	int whitePawnsPosition = 0;
	int blackPawnsPosition = 0;

	Position whiteKingPosition;
	Position blackKingPosition;

	for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 8; y++)
		{
			Position pos(x, y);
			char field = board.GetFieldAtPosition(pos);

			if (field != '0')
			{
				ParseMaterialRate(pos, board, whiteMaterial, blackMaterial);
				ParseKingSecurity(pos, board, whiteKingSecurity, blackKingSecurity);
				ParseDoubledPawns(pos, board, doubledWhitePawnsCount, doubledBlackPawnsCount);
				ParsePawns(pos, board, whitePawnsPosition, blackPawnsPosition);

				if (field == 'k')
					whiteKingPosition = pos;
				if (field == 'K')
					blackKingPosition = pos;
			}
		}
	}

	int doubledPawnsRate = 20;
	int doubledPawnsBalance = 0;
	for (int i = 0; i < 8; i++)
	{
		if (doubledWhitePawnsCount[i] > 1)
			doubledPawnsBalance -= (doubledWhitePawnsCount[i] - 1) * doubledPawnsRate;
		if (doubledBlackPawnsCount[i] > 1)
			doubledPawnsBalance += (doubledBlackPawnsCount[i] - 1) * doubledPawnsRate;
	}

	EGamePhase gamePhase = GetGamePhase(whiteMaterial, blackMaterial, whiteKingPosition, blackKingPosition);
	ParseKingPosition(whiteKingPosition, blackKingPosition, gamePhase, whiteKingPositionRate, blackKingPositionRate);
	ParseBoardControl(board, gamePhase, whiteBoardControl, blackBoardControl);

	int materialBalance = whiteMaterial - blackMaterial;
	int kingSecurityBalance = whiteKingSecurity - blackKingSecurity;
	int kingPositionBalance = whiteKingPositionRate - blackKingPositionRate;
	int boardControlBalance = whiteBoardControl - blackBoardControl;
	int pawnsPositionBalance = whitePawnsPosition - blackPawnsPosition;

	int castlingRate = 10;
	int castlingBalance = (board.CastlingWhiteDone * castlingRate) - (board.CastlingBlackDone * castlingRate);

	if (gamePhase == EGamePhase::Endgame)
	{
		pawnsPositionBalance *= 10;
		kingSecurityBalance /= 10;
	}

	d.MaterialBalance = materialBalance;
	d.KingSecurityBalance = kingSecurityBalance;
	d.KingPositionBalance = kingPositionBalance;
	d.DoubledPawnsBalance = doubledPawnsBalance;
	d.BoardControlBalance = boardControlBalance;
	d.PawnsPositionBalance = pawnsPositionBalance;
	d.GamePhase = gamePhase;

	return materialBalance + kingSecurityBalance + kingPositionBalance + 
							 doubledPawnsBalance + boardControlBalance +
							 pawnsPositionBalance + castlingBalance;
}

int Heuristic::GetPieceValue(char field)
{
	int rate;

	char normalizedField = tolower(field);
	switch (normalizedField)
	{
	case('p'): {rate = 100; break; }
	case('r'): {rate = 500; break; }
	case('n'): {rate = 300; break; }
	case('b'): {rate = 330; break; }
	case('q'): {rate = 1200; break; }
	case('k'): {rate = kingMaterialRate; break; }
	}

	if (field >= 'A' && field <= 'Z')
		rate = -rate;

	return rate;
}

vector<Position> Heuristic::GetFieldAttackersHeuristic(Board board, Position pos, EColor enemyColor)
{
	vector<Position> attackersPositions = board.GetFieldAttackers(pos, enemyColor);
	return attackersPositions;
}

void Heuristic::ParseMaterialRate(Position pos, Board board, int &whiteMaterial, int &blackMaterial)
{
	char field = board.GetFieldAtPosition(pos);
	EColor color = board.GetColorByPieceID(field);
	int fieldValue = GetPieceValue(field);

	if (color == EColor::White)
		whiteMaterial += fieldValue;
	else
		blackMaterial -= fieldValue;
}

void Heuristic::ParseKingSecurity(Position pos, Board board, int &whiteKingSecurity, int &blackKingSecurity)
{
	int shieldRate = 10;
	int shield = 0;

	char field = board.GetFieldAtPosition(pos);
	EColor color = board.GetColorByPieceID(field);

	if (field == 'k' || field == 'K')
	{
		for (int x = -1; x <= 1; x++)
		{
			for (int y = -1; y <= 1; y++)
			{
				Position currentPos = pos + Position(x, y);
				if (!board.IsPositionValid(currentPos))
					continue;

				char field = board.GetFieldAtPosition(currentPos);
				if (color == EColor::White && field == 'p')
					whiteKingSecurity += shieldRate;
				else if (color == EColor::Black && field == 'P')
					blackKingSecurity += shieldRate;
			}
		}
	}
}

EGamePhase Heuristic::GetGamePhase(int whiteMaterial, int blackMaterial, Position whiteKing, Position blackKing)
{
	int allMaterialOnBoard = whiteMaterial + blackMaterial;

	if ((whiteKing != Position(4, 7) || blackKing != Position(4, 0)) && 
		(allMaterialOnBoard <= 7520 && allMaterialOnBoard >= 3000))
		return EGamePhase::Middlegame;
	else if (allMaterialOnBoard < 3000)
		return EGamePhase::Endgame;

	return EGamePhase::Opening;
}

void Heuristic::ParseKingPosition(Position whiteKing, Position blackKing, EGamePhase phase, int &whiteKingPositionRate, int &blackKingPositionRate)
{
	int kingPositionRate = 1;
	int boardRateMiddlegame[8][8] = { { -100, -100, -100, -100, -100, -50, -15, 5 },
										{ -100, -100, -100, -100, -100, -50, -15, 5 },
										{ -100, -100, -100, -100, -100, -50, -15, 10 },
										{ -100, -100, -100, -100, -100, -50, -15, -5 },
										{ -100, -100, -100, -100, -100, -50, -15, 0 },
										{ -100, -100, -100, -100, -100, -50, -15, -10 },
										{ -100, -100, -100, -100, -100, -50, -15, 10 },
										{ -100, -100, -100, -100, -100, -50, -15, 5 }, };

	int boardRateEndgame[8][8] = { { -100, -100,  -100,  -100 , -100,  -100,  -100,  -100 },
									{ -100, -40,   -40,   -40,   -40,   -40,   -40,   -100 },
									{ -100, -40,   0,     0,   0,    0,    -40,   -100 },
									{ -100, -40,   0,     0,   0,    0,    -40,   -100 },
									{ -100, -40,   0,     0,   0,    0,    -40,   -100 },
									{ -100, -40,   0,     0,   0,    0,    -40,   -100 },
									{ -100, -40,   -40,   -40,   -40,   -40,   -40,   -100 },
									{ -100, -100,  -100,  -100 , -100,  -100,  -100,  -100 } };

	if (phase == EGamePhase::Opening || phase == EGamePhase::Middlegame)
	{
		whiteKingPositionRate = kingPositionRate * boardRateMiddlegame[whiteKing.X][whiteKing.Y];
		blackKingPositionRate = kingPositionRate * boardRateMiddlegame[blackKing.X][7 - blackKing.Y];
	}
	else
	{
		whiteKingPositionRate = kingPositionRate * boardRateEndgame[whiteKing.X][whiteKing.Y];
		blackKingPositionRate = kingPositionRate * boardRateEndgame[blackKing.X][7 - blackKing.Y];
	}
}

void Heuristic::ParseDoubledPawns(Position pos, Board board, int(&doubledWhitePawns)[8], int(&doubledBlackPawns)[8])
{
	char field = board.GetFieldAtPosition(pos);
	EColor color = board.GetColorByPieceID(field);

	if (field == 'p')
		doubledWhitePawns[pos.X]++;
	else if (field == 'P')
		doubledBlackPawns[pos.X]++;
}

void Heuristic::ParseBoardControl(Board board, EGamePhase phase, int &whiteBoardControl, int &blackBoardControl)
{
	int boardControlRate2x2 = 1;
	int boardControlRate4x4 = 1;
	int multiplier = 1;

	if (phase == EGamePhase::Opening || phase == EGamePhase::Middlegame)
		multiplier = 1;
	else if (phase == EGamePhase::Endgame)
		multiplier = 0.25;

	for (int x = 3; x <= 4; x++)
	{
		for (int y = 3; y <= 4; y++)
		{
			int rate;
			if ((x == 3 || x == 4) && (y == 3 || y == 4))
				rate = boardControlRate2x2;
			else
				rate = boardControlRate4x4;

			Position pos(x, y);
			whiteBoardControl += board.GetFieldAttackers(pos, EColor::White).size() * rate * multiplier;
			blackBoardControl += board.GetFieldAttackers(pos, EColor::Black).size() * rate * multiplier;
		}
	}
}

void Heuristic::ParsePawns(Position pos, Board board, int &whitePawnsPosition, int &blackPawnsPosition)
{
	int pawnPositionRate = 3;

	char field = board.GetFieldAtPosition(pos);
	EColor color = board.GetColorByPieceID(field);

	if (color == EColor::White)
	{
		whitePawnsPosition += (7 - pos.Y) * pawnPositionRate;
	}
	else
	{
		blackPawnsPosition += (pos.Y) * pawnPositionRate;
	}
}