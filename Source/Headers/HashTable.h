#pragma once
#include <unordered_map>
#include "Headers/TableItem.h"

using namespace std;

class HashTable
{
public:
	HashTable();
	~HashTable();

	//Adds node data with the specific board hash to the map
	void Add(unsigned long long int hash, TableItem item);

	//Returns true if the specific hash exists the in map
	bool Exist(unsigned long long int hash);

	//Returns node data with the specific hash
	TableItem Get(unsigned long long int hash);

	//Clears the map
	void Clear();

private:
	unordered_map<unsigned long long int, TableItem> _table;
};