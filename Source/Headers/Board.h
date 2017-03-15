#pragma once
#include <vector>

#include "Headers/Position.h"
#include "Headers/Enums.h"
#include "Headers/ZobristHash.h"
#include "Headers/Move.h"

using namespace std;

class Board
{
public:
	Board();
	~Board();

	void InitDefaultSettings();
	char GetFieldAtPosition(Position position);
	void Set(Position position, char piece);
	bool IsPositionValid(Position position);
	bool CanKill(Position from, Position to);
	EColor GetColorByPieceID(char id);
	vector<Position> GetFieldAttackers(Position pos, EColor enemyColor);
	Board Inverse();
	vector<Position> FindPiece(char pieceID);
	unsigned long long int GetHash();
	void DoMove(Move move);
	int GetDistance(Position p1, Position p2);
	Move GetMoveByCompare(Board board);

	bool CastlingWhiteDone;
	bool CastlingBlackDone;
	bool CastlingWhiteShort;
	bool CastlingWhiteLong;
	bool CastlingBlackShort;
	bool CastlingBlackLong;

	int MovesCount;
	vector<Move> MovesHistory;
	vector<Position> EnPassantPositions;

private:
	void copyBoard(char source[8][8], char dest[8][8]);
	vector<Position> checkFieldAttackerAtSide(Position pos, EColor enemyColor, int x, int y);
	EMoveCharacterFlags getPieceMoveCharacter(char piece);

	char _board[8][8];
};