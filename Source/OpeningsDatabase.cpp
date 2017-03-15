#include "Headers/OpeningsDatabase.h"

OpeningDatabase::OpeningDatabase()
{

}

OpeningDatabase::~OpeningDatabase()
{

}

void OpeningDatabase::LoadDB()
{
	ifstream db;
	db.open("Databases/openings.txt", ifstream::in);

	if (!db.good())
	{
		return;
	}

	string line;

	while (getline(db, line))
	{
		vector<string> splitted = split(line, '|');
		Opening op;

		op.ID = atoi(splitted[0].c_str());
		op.Category = splitted[1];
		op.Name = splitted[2];

		Board defaultBoard;
		EColor currentColor = EColor::White;

		CoordsConverter converter;
		string moves = splitted[3];
		vector<string> splittedMoves = split(moves, ' ');

		defaultBoard.InitDefaultSettings();

		for (int i = 0; i < splittedMoves.size(); i++)
		{
			string trimMove = splittedMoves[i];
			if (trimMove == "")
				break;

			if (trimMove[0] >= '0' && trimMove[0] <= '9')
			{
				trimMove = trimMove.substr(trimMove.find('.') + 1, trimMove.size() - 2);
			}

			Move convertedMove = converter.FromTrueToBoard(trimMove, currentColor, defaultBoard);
			if (convertedMove.From == Position(0, 0) || convertedMove.To == Position(0, 0))
				break;

			op.Moves.push_back(OpeningMove(defaultBoard.GetHash(), convertedMove));
			defaultBoard.DoMove(convertedMove);
			currentColor = inverseColor(currentColor);
		}

		openings.push_back(op);
	}
	db.close();
}

vector<Opening> OpeningDatabase::GetPossibleOpenings(unsigned long long int boardHash, int moveNumber)
{
	vector<Opening> possibleOpenings;
	for (int i = 0; i < openings.size(); i++)
	{
		if (openings[i].Moves.size() <= moveNumber)
			continue;

		if (openings[i].Moves[moveNumber].BoardHash == boardHash)
		{
			possibleOpenings.push_back(openings[i]);
		}
	}

	return possibleOpenings;
}