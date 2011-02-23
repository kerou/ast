#pragma once

#include <map>
#include <vector>
#include <string>

template <typename T>
struct SymbolTableData
{
    SymbolTableData(char* _string, T _userData):string(_string){userData = _userData;}
    std::string string;
    T userData;
    operator char*(){return (char*)string.c_str();}
};
template <typename T>
class SymbolTable
{
public:
    SymbolTable();
    int lookup(char* string);
    bool add(char* string, T _userSymbolTableData);
    int size();
    SymbolTableData<T>& operator[](int i);
    T& getUserSymbolTableData(int i);
private:
    std::vector<SymbolTableData<T> > strings;
};
template <typename T>
SymbolTable<T>::SymbolTable()
{
}
template <typename T>
int SymbolTable<T>::lookup(char* string)
{
    for (unsigned int i = 0; i < strings.size(); i++)
    {
	if (strings[i].string == string)
	{
	    return i;
	}
    }
    return -1;
}
template <typename T>
bool SymbolTable<T>::add(char* string, T _userSymbolTableData)
{
    if (lookup(string) == -1)
    {
	strings.push_back(SymbolTableData<T>(string,_userSymbolTableData));
	return true;
    }
    else return false;
}
template <typename T>
SymbolTableData<T>& SymbolTable<T>::operator[](int i)
{
    return strings[i];
}
template <typename T>
T& SymbolTable<T>::getUserSymbolTableData(int i)
{
    return strings[i].userSymbolTableData;
}
template <typename T>
int SymbolTable<T>::size()
{
    return strings.size();
}
