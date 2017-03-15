#include "Headers/Board.h"

Board::Board()
{
	
}

Board::~Board()
{

}

void Board::InitDefaultSettings()
{
	char defaultSettingsTemplate[8][8] =
	{
		{ 'R', 'P', '0', '0', '0', '0', 'p', 'r' },
		{ 'N', 'P', '0', '0', '0', '0', 'p', 'n' },
		{ 'B', 'P', '0', '0', '0', '0', 'p', 'b' },
		{ 'Q', 'P', '0', '0', '0', '0', 'p', 'q' },
		{ 'K', 'P', '0', '0', '0', '0', 'p', 'k' },
		{ 'B', 'P', '0', '0', '0', '0', 'p', 'b' },
		{ 'N', 'P', '0', '0', '0', '0', 'p', 'n' },
		{ 'R', 'P', '0', '0', '0', '0', 'p', 'r' }
	};
	
	CastlingWhiteDone = false;
	CastlingBlackDone = false;
	CastlingWhiteShort = true;
	CastlingWhiteLong = true;
	CastlingBlackShort = true;
	CastlingBlackLong = true;

	MovesCount = 0;
	MovesHistory.clear();
	EnPassantPositions.clear();

	copyBoard(defaultSettingsTemplate, _board);
}

char Board::GetFieldAtPosition(Position position)
{
	if (!IsPositionValid(position))
		return '0';
	char r = _board[position.X][position.Y];
	return r;
}

void Board::copyBoard(char source[8][8], char dest[8][8])
{
	for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 8; y++)
		{
			dest[x][y] = source[x][y];
		}
	}
}

void Board::Set(Position position, char piece)
{
	_board[position.X][position.Y] = piece;
}

bool Board::IsPositionValid(Position position)
{
	if (position.X < 0 || position.X > 7 ||
		position.Y < 0 || position.Y > 7)
		return false;
	return true;
}

bool Board::CanKill(Position from, Position to)
{
	char pieceAtFrom = GetFieldAtPosition(from);
	char pieceAtTo = GetFieldAtPosition(to);

	if (pieceAtFrom == '0' || pieceAtTo == '0')
		return false;

	EColor fromPieceColor = GetColorByPieceID(pieceAtFrom);
	EColor toPieceColor = GetColorByPieceID(pieceAtTo);
	if (fromPieceColor != toPieceColor)
		return true;
	else
		return false;
}

EColor Board::GetColorByPieceID(char id)
{
	if (id >= 'a' && id <= 'z')
		return EColor::White;
	else if (id >= 'A' && id <= 'Z')
		return EColor::Black;
	else
		return EColor::None;
}

vector<Position> Board::GetFieldAttackers(Position pos, EColor enemyColor)
{
	vector<Position> attackers;
	vector<Position> possibleMoves = { Position(1, 1),
								   	   Position(-1, -1),
								   	   Position(1, -1),
								 	   Position(-1, 1),
									   Position(0, 1),
									   Position(0, -1),
									   Position(1, 0),
									   Position(-1, 0) };

	for (int i = 0; i < possibleMoves.size(); i++)
	{
		vector<Position> p = checkFieldAttackerAtSide(pos, enemyColor, possibleMoves[i].X, possibleMoves[i].Y);
		if (p.size() != 0) attackers.insert(attackers.end(), p.begin(), p.end());
	}

	//Knight

	vector<Position> knightMoves = { Position(1, 2),
									 Position(-1, 2),
									 Position(1, -2),
									 Position(-1, -2),
									 Position(2, 1),
									 Position(2, -1),
									 Position(-2, 1),
									 Position(-2, -1) };

	for (int i = 0; i < knightMoves.size(); i++)
	{
		Position currentPosition = pos + knightMoves[i];
		char field = GetFieldAtPosition(currentPosition);
		if((field == 'n' && enemyColor == EColor::White) || (field == 'N' && enemyColor == EColor::Black))
			attackers.push_back(currentPosition);
	}

	return attackers;
}

vector<Position> Board::checkFieldAttackerAtSide(Position pos, EColor enemyColor, int x, int y)
{
	vector<Position> enemyPiecesPosition;
	EMoveCharacterFlags currentMoveCharacter;
	Position currentMove;

	if ((x == 1 || x == -1) && (y == 1 || y == -1))
		currentMoveCharacter = EMoveCharacterFlags::Bishop;
	else
		currentMoveCharacter = EMoveCharacterFlags::Rook;

	for (int i = 1; i < 8; i++)
	{
		currentMove = pos + Position(x*i, y*i);
		if (IsPositionValid(currentMove))
		{
			char field = GetFieldAtPosition(currentMove);
			if (field != '0')
			{
				EColor pieceColor = GetColorByPieceID(field);
				EMoveCharacterFlags moveCharacter = getPieceMoveCharacter(field);

				if (pieceColor == enemyColor)
				{
					EMoveCharacterFlags result = currentMoveCharacter & moveCharacter;
					int dist = GetDistance(currentMove, pos);

					if (result != EMoveCharacterFlags::None)
					{
						if ((field == 'p' || field == 'P') && 
							((pieceColor == EColor::Black && pos.Y < currentMove.Y) ||
							(pieceColor == EColor::White && pos.Y > currentMove.Y) ||
							dist != 2)) break;

						if ((field == 'k' || field == 'K') && 
							(((pos.X == currentMove.X || pos.Y == currentMove.Y) && dist >= 2) || 
							((pos.X != currentMove.X && pos.Y != currentMove.Y) && dist >= 3)))
							break;

						enemyPiecesPosition.push_back(currentMove);
					}
					else
						break;
				}
				else
					break;
			}
		}
		else
			break;
	}

	return enemyPiecesPosition;
}

Board Board::Inverse()
{
	Board newBoard;

	for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 8; y++)
		{
			newBoard.Set(Position(x, y), _board[x][7 - y]);
		}
	}

	return newBoard;
}

vector<Position> Board::FindPiece(char pieceID)
{
	vector<Position> result;
	for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 8; y++)
		{
			if (_board[x][y] == pieceID)
				result.push_back(Position(x, y));
		}
	}

	return result;
}

unsigned long long int Board::GetHash()
{
	ZobristHash hashGenerator;

	bool castlingData[4];
	castlingData[0] = CastlingWhiteShort;
	castlingData[1] = CastlingWhiteLong;
	castlingData[2] = CastlingBlackShort;
	castlingData[3] = CastlingBlackLong;

	return hashGenerator.GetHash(_board, castlingData);
}

void Board::DoMove(Move move)
{
	char field = GetFieldAtPosition(move.From);
	EColor pieceColor = GetColorByPieceID(field);

	if (field == 'k')
	{
		if (move.From == Position(4, 7) && move.To == Position(6, 7))
		{
			Set(Position(7, 7), '0');
			Set(Position(5, 7), 'r');
			CastlingWhiteDone = true;
		}
		else 
		if (move.From == Position(4, 7) && move.To == Position(2, 7))
		{
			Set(Position(0, 7), '0');
			Set(Position(3, 7), 'r');
			CastlingWhiteDone = true;
		}

		CastlingWhiteShort = false;
		CastlingWhiteLong = false;
	}
	else
	if (field == 'K')
	{
		if (move.From == Position(4, 0) && move.To == Position(6, 0))
		{
			Set(Position(7, 0), '0');
			Set(Position(5, 0), 'R');
			CastlingBlackDone = true;
		}
		else
		if (move.From == Position(4, 0) && move.To == Position(2, 0))
		{
			Set(Position(0, 0), '0');
			Set(Position(3, 0), 'R');
			CastlingBlackDone = true;
		}

		CastlingBlackShort = false;
		CastlingBlackLong = false;
	}

	if (field == 'r')
	{
		if (move.From == Position(7, 7)) CastlingWhiteShort = false;
		if (move.From == Position(0, 7)) CastlingWhiteLong = false;
	}

	else if (field == 'R')
	{
		if (move.From == Position(7, 0)) CastlingBlackShort = false;
		if (move.From == Position(0, 0)) CastlingBlackLong = false;
	}

	if (tolower(field) == 'p')
	{
		bool enPassantFound = false;
		for (int i = 0; i < EnPassantPositions.size(); i++)
		{
			if (EnPassantPositions[i] == move.To)
			{
				enPassantFound = true;
				break;
			}
		}

		if (enPassantFound)
		{
			if (pieceColor == EColor::White)
			{
				Set(move.To + Position(0, 1), '0');
			}
			else
			{
				Set(move.To + Position(0, -1), '0');
			}
		}
	}

	EnPassantPositions.clear();
	if (tolower(field) == 'p')
	{
		if (abs(move.From.Y - move.To.Y) == 2)
		{
			if (pieceColor == EColor::White)
			{
				EnPassantPositions.push_back(move.To + Position(0, 1));
			}
			else
			{
				EnPassantPositions.push_back(move.To + Position(0, -1));
			}
		}
	}

	if (move.IsPromotion)
	{
		field = move.PromotionPiece;
	}

	Set(move.From, '0');
	Set(move.To, field);

	MovesCount++;
	MovesHistory.push_back(move);
}

EMoveCharacterFlags Board::getPieceMoveCharacter(char piece)
{
	char fixedPiece = tolower(piece);
	switch (fixedPiece)
	{
	case('p'): return EMoveCharacterFlags::Bishop;
	case('r'): return EMoveCharacterFlags::Rook;
	case('n'): return EMoveCharacterFlags::Knight;
	case('b'): return EMoveCharacterFlags::Bishop;
	case('q'): return EMoveCharacterFlags::Rook | EMoveCharacterFlags::Bishop;
	case('k'): return EMoveCharacterFlags::Rook | EMoveCharacterFlags::Bishop;
	}
}

int Board::GetDistance(Position p1, Position p2)
{
	return abs(p1.X - p2.X) + abs(p1.Y - p2.Y);
}

Move Board::GetMoveByCompare(Board board)
{
	Move m;

	

	return m;
}