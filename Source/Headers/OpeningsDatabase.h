#pragma once
#include <string>
#include <vector>
#include <fstream>
#include "Headers/OpeningDBResult.h"
#include "Headers/Split.h"
#include "Headers/CoordsConverter.h"
#include "Headers/ColorOperations.h"

using namespace std;

class OpeningDatabase
{
public:
	OpeningDatabase();
	~OpeningDatabase();

	void LoadDB();
	vector<Opening> GetPossibleOpenings(unsigned long long int boardHash, int moveNumber);

private:
	vector<Opening> openings;
};