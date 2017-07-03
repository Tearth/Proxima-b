#include "Headers/ConfigLoader.h"

ConfigLoader::ConfigLoader()
{

}

ConfigLoader::~ConfigLoader()
{

}

bool ConfigLoader::LoadFromFile(string fileName)
{
	ifstream file(fileName);
	if (!file.is_open())
		return false;

	string line;
	while (!file.eof())
	{
		getline(file, line);
		int separatePosition = line.find_first_of('=');

		string key = line.substr(0, separatePosition);
		string value = line.substr(separatePosition + 1, line.size() - separatePosition - 1);

		values[key] = value;
	}

	file.close();
}

string ConfigLoader::GetValueOfKey(string key)
{
	return values[key];
}