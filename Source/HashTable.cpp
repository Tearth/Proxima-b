#include "Headers/HashTable.h"

HashTable::HashTable()
{

}

HashTable::~HashTable()
{

}

void HashTable::Add(unsigned long long int hash, TableItem item)
{
	_table[hash] = item;
}

bool HashTable::Exist(unsigned long long int hash)
{
	if (_table.find(hash) == _table.end())
		return false;
	return true;
}

TableItem HashTable::Get(unsigned long long int hash)
{
	return _table[hash];
}

void HashTable::Clear()
{
	_table.clear();
}