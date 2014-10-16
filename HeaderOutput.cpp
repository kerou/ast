#include "ParserOutputEgg.hpp"
#include "SymbolTable.hpp"
#define ALIASING
#include <sstream>
using namespace std;
extern std::unordered_map<std::string,void*> characterLiterals;
extern std::unordered_map<std::string,Token_Rule*> terminalTokens;
extern std::unordered_map<std::string,Symbol_Rule_List*> listRules;
extern std::unordered_map<std::string,Symbol_Rule_STRING*> stringTokens;
extern std::unordered_map<std::string,Token_Rule2*> stringAliases;
void outputListTypeDeclarations(ofstream* file)
{
  auto iter = listRules.begin();
  while (iter != listRules.end())
    {
      (*file) << "class " << iter->first << "_List;" << endl;
      iter++;
    }
}
void outputListTypes(ofstream* file)
{
  auto iter = listRules.begin();
  while (iter != listRules.end())
    {
      (*file) << "class " << iter->first << "_List: public ListRuleBaseClass" << endl;
      (*file) << '{' << endl;
      (*file) << "public:" << endl;
      (*file) << '\t' << iter->first << "_List();" << endl;
      (*file) << "\tRuleType type(){return e" << iter->first << "_List;}" << endl;
      (*file) << "\tvoid add(" << iter->first << "_Type*);" << endl;
      (*file) << "\tunsigned int size(){return list.size();}" << endl;
      (*file) << '\t' << iter->first << "_Type* operator[](unsigned int index){return list[index];}" << endl;
      (*file) << "private:" << endl;
      (*file) << "\tstd::vector<" << iter->first << "_Type*> list;" << endl;
      (*file) << "};" << endl;
      iter++;
    }
}
void outputFile(ofstream* outfile, const char* infileName);
void ParserOutput::outputHeader()
{
  ofstream file(outputfile + ".h");
  file << "#pragma once" << endl;
    
  file << "/// Enums" << endl;
  file << "enum RuleType" << endl;
  file << '{' << endl;
  rules->outputEnumDeclarations(&file);
  auto iter = terminalTokens.begin();
  while (iter != terminalTokens.end())
    {
      file << "\te" << iter->first << ',' << endl;
      iter++;
    }
  auto stringIter = stringTokens.begin();
  while (stringIter != stringTokens.end())
    {
      file << "\te" << stringIter->first << "_String," << endl;
      stringIter++;
    }
  auto listIter = listRules.begin();
  while (listIter != listRules.end())
  {
    file << "\te" << listIter->first << "_List," << endl;
    listIter++;
  }
  file << "\teCharacterLiteralsBegin," << endl;
  file << "\teTypesMax = eCharacterLiteralsBegin + 256" << endl;
  file << "};" << endl;
  {
    std::string fullFilename(resourceDirectory);
    fullFilename = fullFilename + "astres/HeaderPreamble.txt";
    outputFile(&file,fullFilename.c_str());
  }  
  file << "/// Forward declarations of all types" << endl;
  rules->outputTypeDeclarations(&file);
  file << "/// List rule types" << endl;
  outputListTypeDeclarations(&file);
  file << "/// Terminal token aliases" << endl;
  iter = terminalTokens.begin();
  while (iter != terminalTokens.end())
    {
      file << "class " << iter->first << "_Type;" << endl;
      iter++;
    }
  stringIter = stringTokens.begin();
  while (stringIter != stringTokens.end())
    {
      file << "class " << stringIter->first << "_String_Type;" << endl;
      stringIter++;
    }
  file << "/// Character literal" << endl;
  file << "class CharacterLiteral_Type;" << endl;
  
  /// Type definitions
  file << "/// Actual declarations" << endl;
  file << "/// User defined rules" << endl;
  rules->outputTypes(&file);
  file << "/// List rules types" << endl;
  outputListTypes(&file);
  file << "/// Terminal tokens types" << endl;
  iter = terminalTokens.begin();
  while (iter != terminalTokens.end())
    {
      file << "class " << iter->first << "_Type: public TerminalTokenBaseClass" << endl;
      file << '{' << endl;
      file << "public:" << endl;
      file << '\t' << iter->first << "_Type(char* _matchedText, char* _preceedingWhitespace);" << endl;
      file << "\tRuleType type(){return e" << iter->first << ";}" << endl;
      file << "};" << endl;
      iter++;
    }
  stringIter = stringTokens.begin();
  while (stringIter != stringTokens.end())
    {
      file << "class " << stringIter->first << "_String_Type: public TerminalTokenBaseClass" << endl;
      file << '{' << endl;
      file << "public:" << endl;
      file << '\t' << stringIter->first << "_String_Type(char* _matchedText, char* _preceedingWhitespace);" << endl;
      file << "\tRuleType type(){return e" << stringIter->first << "_String;}" << endl;
      file << "};" << endl;
      stringIter++;
    }
  file << "Script_Type* parse(const char* filename, bool outputMatchedTokens = false);" << endl;
}
void BisonRules_Rule::outputEnumDeclarations(ofstream* file)
{
  for (unsigned int i = 0; i < rules.size(); i++)
    {
      rules[i]->outputEnumDeclaration(file);
    }
}
void BisonRule_Rule::outputEnumDeclaration(ofstream* file)
{
  (*file) << "\te" << name << ',' << endl;
}
void BisonRules_Rule::outputTypeDeclarations(ofstream* file)
{
  for (unsigned int i = 0; i < rules.size(); i++)
    {
      rules[i]->outputTypeDeclaration(file);
    }
}
void BisonRule_Rule::outputTypeDeclaration(ofstream* file)
{
  (*file) << "class " << name << "_Type;" << endl;
  if (rules->size() != 1)
    {
      for (unsigned int i = 0; i < rules->size(); i++)
        {
          (*file) << "class " << name << "_Type" << i+1 << ';' << endl;
        }
    }
}
void BisonRules_Rule::outputTypes(ofstream* file)
{
  for (unsigned int i = 0; i < rules.size(); i++)
    {
      rules[i]->outputType(file);
    }
}
void BisonRule_Rule::outputType(ofstream* file)
{
  rules->outputType(file,name);
}
void DerivationRules_Rule::outputType(ofstream* file, char* type)
{
  stringstream derivationName;
  derivationName << type << "_Type";
  std::string typeName = derivationName.str();
  (*file) << "class " << typeName << ": public NonTerminalBaseClass" << endl;
  (*file) << '{' << endl;
  (*file) << "public:" << endl;
  (*file) << '\t' << typeName << '(';
  if (rules.size() == 1)
    rules[0]->outputConstructorArguments(file);
  else
    (*file) << "const unsigned short _numargs, const unsigned short _derivationType";
  (*file) << ");" << endl;
  (*file) << "\tvirtual ~" << typeName << "();" << endl;
  if (rules.size() == 1)
    rules[0]->outputGetDeclarations(file);
  else
    (*file) << "\tunsigned short getType(){return derivationType;}" << endl;
  (*file) << "\tRuleType type(){return e" << type << ";}" << endl;
  (*file) << "private:" << endl;
  if (rules.size() != 1)
    (*file) << "\tunsigned short derivationType;" << endl;
  (*file) << "};" << endl;
  if (rules.size() != 1)
    {
      for (unsigned int i = 0; i < rules.size(); i++)
        {
          (*file) << "class " << typeName << i+1 << ": public " << typeName << endl;
          (*file) << '{' << endl;
          (*file) << "public:" << endl;
          (*file) << "\t" << typeName << i+1 << '(';
          rules[i]->outputConstructorArguments(file);
          (*file) << ");" << endl;
          (*file) << "\tvirtual ~" << typeName << i+1 << "();" << endl;
          rules[i]->outputGetDeclarations(file);
          (*file) << "private:" << endl;
          (*file) << "};" << endl;
        }
    }
}
void Symbols_Rule::outputConstructorArguments(ofstream* file)
{
  for (unsigned int i = 0; i < symbols.size(); i++)
    {
      symbols[i]->outputType(file);
      (*file) << "*_arg" << i+1;
      if (i < symbols.size()-1)
        {
          (*file) << ", ";
        }
    }
}
#include <cassert>
void Symbols_Rule::outputMemberDeclarations(ofstream* file)
{
  assert(false); /// FIXME just remove all these functions
  for (unsigned int i = 0; i < symbols.size(); i++)
    {
      (*file) << '\t';
      symbols[i]->outputType(file);
      (*file) << "*arg" << i+1 << ';' << endl;
    }
}
void Symbols_Rule::outputGetDeclarations(ofstream* file)
{
  for (unsigned int i = 0; i < symbols.size(); i++)
    {
      (*file) << '\t';
      symbols[i]->outputType(file);
      (*file) << "* getArg" << i+1 << "(){return (";
      symbols[i]->outputType(file);
      (*file) << "*)args[" << i << "];}" << endl;
    }
}
void Symbol_Rule_ID::outputType(ofstream* file)
{
  if (terminalTokens.find(name) == terminalTokens.end())
    (*file) << name << "_Type ";
  else
    (*file) << name << "_Type ";
}
void Symbol_Rule_CHARACTER::outputType(ofstream* file)
{
  /*
  #ifndef ALIASING
     (*file) << '"' << character << '"' << ' ';
  #else
     int literalInt = character;
     (*file) << "LitChar" << literalInt << ' ';
  #endif*/
  (*file) << "CharacterLiteral_Type ";
}
void Symbol_Rule_STRING::outputType(ofstream* file)
{
  (*file) << string << "_String_Type ";
  //  Token_Rule2* token = stringAliases.find(string)->second;
  //  (*file) << token->getId() << "_Type" << ' ';
}
void Symbol_Rule_List::outputType(ofstream* file)
{
  (*file) << name << "_List ";
}
#include <cassert>
void Token_Rule::outputConstructorArguments(ofstream* file)
{
  assert(false);
  /// Will always be (char* matchedText, char* preceedingWhitespace)
}



















