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

	Move FromTrueToBoard(string trueCoord, EColor color, Board board);
	Position ConvertCoordinates(string coords);
	string CovertFromBoardToTrue(Position pos);
	char ConvertPolishToEnglishPieceID(char id);
	Board ConvertFromFICSFormatToBoard(string ficsFormat);

private:
	Position getMovablePieceFromField(Board board, char piece, Position coordsTo, EColor color);
};