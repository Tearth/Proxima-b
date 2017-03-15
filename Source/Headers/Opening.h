#pragma once
#include <string>
#include <vector>
#include "Headers/OpeningMove.h"

using namespace std;

class Opening
{
public:
	Opening();
	~Opening();

	int ID;
	string Category;
	string Name;
	vector<OpeningMove> Moves;
};