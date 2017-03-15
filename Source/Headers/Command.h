#pragma once
#include <string>
#include <vector>

#include "Headers/Enums.h"

using namespace std;

class Command
{
public:
	Command(ECommandType type, vector<string> args);
	~Command();

	ECommandType Type;
	vector<string> Arguments;
};