#include "Headers/CoordsConverter.h"

CoordsConverter::CoordsConverter()
{

}

CoordsConverter::~CoordsConverter()
{

}

Move CoordsConverter::FromTrueToBoard(string trueCoord, EColor color, Board board)
{
	char piece, x, y;
	bool kill = false;

	char dir = color == EColor::White ? 1 : -1;

	if (trueCoord == "O-O")
	{
		if (color == EColor::White)
			return Move(Position(4, 7), Position(6, 7));
		else
			return Move(Position(4, 0), Position(6, 0));
	}
	else if (trueCoord == "O-O-O")
	{
		if (color == EColor::White)
			return Move(Position(4, 7), Position(2, 7));
		else
			return Move(Position(4, 0), Position(2, 0));
	}
	else if (trueCoord.length() == 2)
	{
		piece = 'p';
		x = trueCoord[0];
		y = trueCoord[1];

		Position from = ConvertCoordinates(trueCoord);
		Position p = from;
		while (board.IsPositionValid(p))
		{
			p = p + (Position(0, 1) * dir);
			if (tolower(board.GetFieldAtPosition(p)) == 'p')
			{
				return Move(p, from);
			}
		}

	}
	else if (trueCoord.length() == 3)
	{
		piece = trueCoord[0];
		x = trueCoord[1];
		y = trueCoord[2];

		piece = ConvertPolishToEnglishPieceID(piece);

		if (color == EColor::White)
			piece = tolower(piece);
		else
			piece = toupper(piece);

		Position to = ConvertCoordinates(trueCoord.substr(1, 2));
		Position validPiece = getMovablePieceFromField(board, piece, to, color);

		return Move(validPiece, to);
	}
	else if (trueCoord.length() == 4)
	{
	char tmpPiece = trueCoord[0];
	if (tmpPiece >= 'a' && tmpPiece <= 'h')
		piece = 'p';
	else
		piece = trueCoord[0];

	x = trueCoord[2];
	y = trueCoord[3];
	kill = true;

	piece = ConvertPolishToEnglishPieceID(piece);
	if (color == EColor::White)
		piece = tolower(piece);
	else
		piece = toupper(piece);

	Position to = ConvertCoordinates(trueCoord.substr(2, 2));
	Position validPiece = getMovablePieceFromField(board, piece, to, color);

	return Move(validPiece, to);
	}

	return Move();
}

Position CoordsConverter::getMovablePieceFromField(Board board, char piece, Position coordsTo, EColor color)
{
	vector<Position> pieces = board.FindPiece(piece);
	PieceBase* pieceBase = PiecesManager::GetPieceByID(piece);
	if (pieceBase->GetID() == 'p')
		dynamic_cast<Pawn*>(pieceBase)->SetEnPassantMoves(board.EnPassantPositions);

	Position validPiece;

	for (int i = 0; i < pieces.size(); i++)
	{
		vector<Move> availableMoves = pieceBase->GetAvailableMoves(board, pieces[i], color);

		bool found = false;
		for (int x = 0; x < availableMoves.size(); x++)
		{
			if (availableMoves[x].To == coordsTo)
			{
				found = true;
				validPiece = pieces[i];
				break;
			}
		}

		if (found)
			break;
	}

	return validPiece;
}

Position CoordsConverter::ConvertCoordinates(string coords)
{
	char x = coords[0];
	char y = coords[1];

	int a, b;

	a = x - 'a';
	b = 8 - (y - '0');
	return Position(a, b);
}

string CoordsConverter::CovertFromBoardToTrue(Position pos)
{
	string result;
	result += 'a' + pos.X;
	result += '0' + 8 - pos.Y;

	return result;
}

char CoordsConverter::ConvertPolishToEnglishPieceID(char id)
{
	id = tolower(id);
	switch (id)
	{
	case('w'): return 'r';
	case('s'): return 'n';
	case('g'): return 'b';
	case('h'): return 'q';
	case('k'): return 'k';
	}

	return id;
}

Board CoordsConverter::ConvertFromFICSFormatToBoard(string ficsFormat)
{
	Board b;

	int i = 0;
	for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 8; y++)
		{
			char p = ficsFormat[i];
			if (p == '-')
			{
				p = '0';
			}
			else
			{
				if (p >= 'A' && p <= 'Z')
					p = tolower(p);
				else
					p = toupper(p);
			}

			b.Set(Position(y, x), p);
			i++;
		}

		i++;
	}

	return b;
}