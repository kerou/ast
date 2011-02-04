#pragma once
class BisonRules_Rule;
class UnionMembers_Rule;
extern class ParserOutput
{
public:
    ParserOutput();
    void addRules(BisonRules_Rule* _rules);
    void addUnion(UnionMembers_Rule* _unionDef);
private:
    BisonRules_Rule* rules;
    UnionMembers_Rule* unionDef;
}g_ParserOutput;
#include <vector>
using namespace std;

/// **** RULES SECTION ****
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

/// **** UNION SECTION ***
class UnionMember_Rule
{
public:
    UnionMember_Rule();
};
class UnionMembers_Rule
{
public:
    UnionMembers_Rule();
    UnionMembers_Rule(UnionMember_Rule* member);
    void addMember(UnionMember_Rule* member);
private:
    vector<UnionMember_Rule*> members;
};
class UnionMember_Rule1: public UnionMember_Rule
{
public:
    UnionMember_Rule1(char* _typename, char* _name);
private:
    char* typeName,* name;
};
class UnionMember_Rule2: public UnionMember_Rule
{
public:
    UnionMember_Rule2(char* _typename, char* _name);
private:
    char* typeName,* name;
};
