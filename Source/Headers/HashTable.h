#pragma once
#include <unordered_map>
#include "Headers/TableItem.h"

using namespace std;

class HashTable
{
public:
	HashTable();
	~HashTable();

	void Add(unsigned long long int hash, TableItem item);
	bool Exist(unsigned long long int hash);
	TableItem Get(unsigned long long int hash);
	void Clear();

private:
	unordered_map<unsigned long long int, TableItem> _table;
};