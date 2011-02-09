#pragma once

#include <map>
#include <vector>
#include <string>

class SymbolTable
{
public:
    SymbolTable();
    int lookup(char* string);
    bool add(char* string);
    int size();
    const char* operator[](int i);
private:
    std::vector<std::string> strings;
};
