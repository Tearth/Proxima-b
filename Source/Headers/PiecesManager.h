#pragma once
#include <vector>
#include <ctype.h>

#include "Headers/PieceBase.h"
#include "Headers/Pawn.h"
#include "Headers/Rook.h"
#include "Headers/Knight.h"
#include "Headers/Bishop.h"
#include "Headers/Queen.h"
#include "Headers/King.h"

using namespace std;

static class PiecesManager
{
public:
	static void Init();
	static void Delete();

	static PieceBase* GetPieceByID(char id);
	static bool IsPieceValid(char piece);

private:
	static vector<PieceBase*> _pieces;
};