#pragma once
#include <string>
#include <sstream>
#include <vector>

using namespace std;

void split(const string &s, char splitter, vector<string> &elems);
vector<string> split(const string &s, char splitter);
string tolowerString(string str);