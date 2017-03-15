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

	//Reads configuration from the specific file
	bool LoadFromFile(string fileName);

	//Returns value of the specific key
	string GetValueOfKey(string key);

private:
	map<string, string> values;
};