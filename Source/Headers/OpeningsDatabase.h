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

	//Reads file and fills the openings vector
	void LoadDB();

	//Returns vector of possible openings for the specifis board hash
	vector<Opening> GetPossibleOpenings(unsigned long long int boardHash, int moveNumber);

private:
	vector<Opening> openings;
};