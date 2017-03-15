#pragma once
#include "Headers/Move.h"
#include "Headers/Board.h"
#include "Headers/PieceBase.h"
#include "Headers/PiecesManager.h"

class CoordsConverter
{
public:
	CoordsConverter();
	~CoordsConverter();

	//Converts standard notation to board position
	Move FromTrueToBoard(string trueCoord, EColor color, Board board);

	//Converts standard notation to internal
	Position ConvertCoordinates(string coords);

	//Converts internal notation to standard
	string CovertFromBoardToTrue(Position pos);

	//Converts to English piece symbol
	char ConvertPolishToEnglishPieceID(char id);

	//Converts from FICS notation to internal
	Board ConvertFromFICSFormatToBoard(string ficsFormat);

private:
	Position getMovablePieceFromField(Board board, char piece, Position coordsTo, EColor color);
};