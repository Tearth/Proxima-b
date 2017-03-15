#include "Headers/Split.h"

void split(const string &s, char splitter, vector<string> &elems) {
	stringstream ss;
	ss.str(s);
	string item;
	while (std::getline(ss, item, splitter)) {
		elems.push_back(item);
	}
}


vector<string> split(const string &s, char splitter) {
	vector<string> elems;
	split(s, splitter, elems);
	return elems;
}

string tolowerString(string str)
{
	for (int i = 0; i < str.size(); i++)
		str[i] = tolower(str[i]);
	return str;
}