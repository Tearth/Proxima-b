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

	//Sets board state to default values
	void InitDefaultSettings();

	//Returns symbol of an piece at the specific field (or ' ')
	char GetFieldAtPosition(Position position);

	//Sets piece symbol at the specific position
	void Set(Position position, char piece);

	//Returns true if the specific position is valid
	bool IsPositionValid(Position position);

	//Returns true if piece at the specific position can kill an another piece 
	bool CanKill(Position from, Position to);

	//Returns color of piece
	EColor GetColorByPieceID(char id);

	//Returns list of pieces which are attaking the specified position
	vector<Position> GetFieldAttackers(Position pos, EColor enemyColor);

	//Inverses the board
	Board Inverse();

	//Returns positions of the specified piece
	vector<Position> FindPiece(char pieceID);

	//Returns hash
	unsigned long long int GetHash();

	//Does move
	void DoMove(Move move);

	//Returns distance from p1 to p2
	int GetDistance(Position p1, Position p2);

	//Returns move using difference between boards
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