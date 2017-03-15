#include "Headers/ZobristHash.h"

bool ZobristHash::_isInit = false;
unsigned long long int ZobristHash::_randomTable[8][8][12];
unsigned long long int ZobristHash::_castlingTable[4];

ZobristHash::ZobristHash()
{
	if (!_isInit)
	{
		std::random_device randomDevice;
		std::mt19937_64 gen64bit(randomDevice());
		std::uniform_int_distribution<unsigned long long> dis(0, UINT64_MAX);

		for (int x = 0; x < 8; x++)
		{
			for (int y = 0; y < 8; y++)
			{
				for (int i = 0; i < 12; i++)
				{
					_randomTable[x][y][i] = dis(gen64bit);
				}
			}
		}

		for (int i = 0; i < 4; i++)
		{
			_castlingTable[i] = dis(gen64bit);
		}

		_isInit = true;
	}
}

ZobristHash::~ZobristHash()
{

}

unsigned long long int ZobristHash::GetHash(char boardArray[8][8], bool castlingArray[4])
{
	unsigned long long int hash = 0;

	for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 8; y++)
		{
			if (boardArray[x][y] != '0')
			{
				int pieceValue = getPieceValue(boardArray[x][y]);
				hash ^= _randomTable[x][y][pieceValue];
			}
		}
	}

	for (int i = 0; i < 4; i++)
	{
		if(castlingArray[i])
			hash ^= _castlingTable[i];
	}

	return hash;
}

int ZobristHash::getPieceValue(char pieceID)
{
	switch (pieceID)
	{
	case('p'): return 0;
	case('P'): return 1;
	case('r'): return 2;
	case('R'): return 3;
	case('n'): return 4;
	case('N'): return 5;
	case('b'): return 6;
	case('B'): return 7;
	case('q'): return 8;
	case('Q'): return 9;
	case('k'): return 10;
	case('K'): return 11;
	}

	return -1;
}