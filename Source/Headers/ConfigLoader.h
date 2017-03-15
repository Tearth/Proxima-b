#pragma once
#include <string>
#include <fstream>
#include <map>

using namespace std;

class ConfigLoader
{
public:
	ConfigLoader();
	~ConfigLoader();

	bool LoadFromFile(string fileName);
	string GetValueOfKey(string key);

private:
	map<string, string> values;
};