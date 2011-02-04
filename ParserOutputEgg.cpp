#include "ParserOutputEgg.h"
#include <cstring>
ParserOutput g_ParserOutput;
Symbols_Rule::Symbols_Rule(void)
{
}
Symbols_Rule::Symbols_Rule(Symbol_Rule* symbol)
{
  symbols.push_back(symbol);
}
void Symbols_Rule::addSymbol(Symbol_Rule* symbol)
{
  symbols.push_back(symbol);
}

DerivationRules_Rule::DerivationRules_Rule(Symbols_Rule* symbols)
{
  rules.push_back(symbols);
}
void DerivationRules_Rule::addRule(Symbols_Rule* symbols)
{
  rules.push_back(symbols);
}

BisonRule_Rule::BisonRule_Rule(char* _name, DerivationRules_Rule* _rules)
{
  name = _name;
  rules = _rules;
}

BisonRules_Rule::BisonRules_Rule(BisonRule_Rule* rule)
{
  rules.push_back(rule);
}
void BisonRules_Rule::addRule(BisonRule_Rule* rule)
{
  rules.push_back(rule);
}

Symbol_Rule_ID::Symbol_Rule_ID(char* _name)
{
  name = _name;
}

Symbol_Rule_CHARACTER::Symbol_Rule_CHARACTER(char _character)
{
  character = _character;
}

Symbol_Rule_STRING::Symbol_Rule_STRING(char* _string)
{
  string = _string;
}

UnionMembers_Rule::UnionMembers_Rule()
{
}
UnionMembers_Rule::UnionMembers_Rule(UnionMember_Rule* rule)
{
   members.push_back(rule);
}
void UnionMembers_Rule::addMember(UnionMember_Rule* rule)
{
    members.push_back(rule);
}
UnionMember_Rule::UnionMember_Rule()
{
}

UnionMember_Rule1::UnionMember_Rule1(char* _typename, char* _name)
{
    typeName = _typename;
    name = _name;
}

UnionMember_Rule2::UnionMember_Rule2(char* _typename, char* _name)
{
    name = _name;
    typeName = new char[strlen(_typename)+1];
    strcpy(typeName,_typename);
    typeName[strlen(_typename)] = '*';
}
/// SemanticActions
ParserOutput::ParserOutput()
{
    rules = NULL;
    unionDef = NULL;
}
void ParserOutput::addRules(BisonRules_Rule* _rules)
{
    rules = _rules;
}
void ParserOutput::addUnion(UnionMembers_Rule* _unionDef)
{
    unionDef = _unionDef;
}
