#include "Headers/PiecesManager.h"

vector<PieceBase*> PiecesManager::_pieces;

void PiecesManager::Init()
{
	_pieces.push_back(new Pawn());
	_pieces.push_back(new Rook());
	_pieces.push_back(new Knight());
	_pieces.push_back(new Bishop());
	_pieces.push_back(new Queen());
	_pieces.push_back(new King());
}

PieceBase* PiecesManager::GetPieceByID(char id)
{
	/*
	Pieces IDs:
	Name		|	Black	|	White
	-----------------------------------
	Pawn		|	P		|	p
	Knight		|	N		|	n
	Bishop		|	B		|	b
	Rook		|	R		|	r
	Queen		|	Q		|	q
	King		|	K		|	k
	*/

	char lowerID = tolower(id);
	
	for (int i = 0; i < _pieces.size(); i++)
	{
		if (_pieces[i]->GetID() == lowerID)
			return _pieces[i];
	}

	return 0;
}

bool PiecesManager::IsPieceValid(char piece)
{
	if (GetPieceByID(piece) != 0)
		return true;
	else
		return false;
}

void PiecesManager::Delete()
{
	for (int i = 0; i < _pieces.size(); i++)
		delete _pieces[i];
}