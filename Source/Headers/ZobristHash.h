#pragma once
#include <random>

using namespace std;

class ZobristHash
{
public:
	ZobristHash();
	~ZobristHash();

	//Generate hash based on board data and castling parameters
	unsigned long long int GetHash(char boardArray[8][8], bool castlingArray[4]);
private:
	static unsigned long long int _randomTable[8][8][12];
	static unsigned long long int _castlingTable[4];
	static bool _isInit;
		
	int getPieceValue(char pieceID);
};