#include "ParserOutputEgg.hpp"
#include "SymbolTable.hpp"
#include <cstring>
extern std::unordered_map<std::string,LexMode_Rule*> lexModeTable;
extern std::unordered_map<std::string,Symbol_Rule_STRING*> stringTokens;
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

Symbol_Rule_STRING::Symbol_Rule_STRING(char* _string)
{
  string = _string;
  stringTokens[string] = this;
}

/// SemanticActions
ParserOutput::ParserOutput()
{
  rules = NULL;
  declarations = NULL;
  lexModes = NULL;
}
void ParserOutput::addRules(BisonRules_Rule* _rules)
{
  rules = _rules;
}
void ParserOutput::addDeclarations(BisonDeclarations_Rule* _declarations)
{
  declarations = _declarations;
}
void ParserOutput::addLexModes(LexModes_Rule* _lexModes)
{
  lexModes = _lexModes;
}
void ParserOutput::output()
{
  outputBison();
  outputHeader();
  outputCpp();
  outputFlex();
}
/// Lex rules
void LexModes_Rule::add(LexMode_Rule* mode)
{
  modes.push_back(mode);
}
LexMode_Rule::LexMode_Rule(char* _id, LexRules_Rule* _rules)
{
  id = _id;
  rules = _rules;
  lexModeTable[id] = this;
}
void LexRules_Rule::add(LexRule_Rule* rule)
{
  rules.push_back(rule);
}
LexRule_Rule::LexRule_Rule(char* _id, LexCommand_Rule* _command)
{
  id = _id;
  command = _command;
}
void LexCommand_Rule::add(char* regex)
{
  regexes.push_back(regex);
}


















