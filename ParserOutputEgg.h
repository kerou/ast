#pragma once
extern class ParserOutput
{
public:
    ParserOutput();
private:
}g_ParserOutput;
#include <vector>
using namespace std;

class Symbol_Rule
{
};
class Symbols_Rule
{
public:
    Symbols_Rule(void);
    Symbols_Rule(Symbol_Rule* symbol);
    void addSymbol(Symbol_Rule* symbol);
private:
    vector<Symbol_Rule*> symbols;
};
class DerivationRules_Rule
{
public:
    DerivationRules_Rule(Symbols_Rule* symbols);
    void addRule(Symbols_Rule* symbols);
private:
    vector<Symbols_Rule*> rules;
};
class BisonRule_Rule
{
public:
    BisonRule_Rule(char* _name, DerivationRules_Rule* _rules);
private:
    char* name;
    DerivationRules_Rule* rules;
};
class BisonRules_Rule
{
public:
    BisonRules_Rule(BisonRule_Rule* rule);
    void addRule(BisonRule_Rule* rule);
private:
    vector<BisonRule_Rule*> rules;
};


/// Derived possibilities
class Symbol_Rule_ID: public Symbol_Rule
{
public:
    Symbol_Rule_ID(char* _name);
private:
    char* name;
};
class Symbol_Rule_CHARACTER: public Symbol_Rule
{
public:
    Symbol_Rule_CHARACTER(char _character);
private:
    char character;
};
class Symbol_Rule_STRING: public Symbol_Rule
{
public:
    Symbol_Rule_STRING(char* _string);
private:
    char* string;
};
