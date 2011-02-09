#include "SymbolTable.h"
SymbolTable::SymbolTable()
{
}
int SymbolTable::lookup(char* string)
{
    for (int i = 0; i < strings.size(); i++)
    {
	if (strings[i] == string)
	{
	    return i;
	}
    }
    return -1;
}
bool SymbolTable::add(char* string)
{
    if (lookup(string) == -1)
    {
	strings.push_back(string);
	return true;
    }
    else return false;
}
const char* SymbolTable::operator[](int i)
{
    return strings[i].c_str();
}
int SymbolTable::size()
{
    return strings.size();
}
