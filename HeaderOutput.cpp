#include "ParserOutputEgg.h"
#include "SymbolTable.h"
#define ALIASING
#include <sstream>
using namespace std;
extern SymbolTable<void*> characterLiterals;
extern SymbolTable<Token_Rule*> terminalTokens;
extern SymbolTable<Symbol_Rule_List*> listRules;
extern SymbolTable<Token_Rule2*> stringAliases;
void outputListTypeDeclarations(ofstream* file)
{
    for (int i = 0; i < listRules.size(); i++)
    {
        (*file) << "class " << listRules[i] << "_List;" << endl;
    }
}
void outputListTypes(ofstream* file)
{
    for (int i = 0; i < listRules.size(); i++)
    {
        (*file) << "class " << listRules[i] << "_List: public ListRuleBaseClass" << endl;
        (*file) << '{' << endl;
        (*file) << "public:" << endl;
        (*file) << '\t' << listRules[i] << "_List();" << endl;
        (*file) << "\tvoid add(" << listRules[i] << "_Type*);" << endl;
        (*file) << "\tunsigned int size(){return list.size();}" << endl;
        (*file) << '\t' << listRules[i] << "_Type* operator[](unsigned int index){return list[index];}" << endl;
        (*file) << "private:" << endl;
        (*file) << "\tstd::vector<" << listRules[i] << "_Type*> list;" << endl;
        (*file) << "};" << endl;
    }
}
void outputFile(ofstream* outfile, const char* infileName);
void ParserOutput::outputHeader()
{
    ofstream file("TestOutput.h");
    outputFile(&file,"HeaderPreamble.txt");
    file << "/// Forward declarations of all types" << endl;
    rules->outputTypeDeclarations(&file);
    file << "/// List rule types" << endl;
    outputListTypeDeclarations(&file);
    file << "/// Terminal token aliases" << endl;
    for (int i = 0; i < terminalTokens.size(); i++)
    {
        file << "class " << terminalTokens[i].string << "_Type;" << endl;
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
    for (int i = 0; i < terminalTokens.size(); i++)
    {
        file << "class " << terminalTokens[i].string << "_Type: public TerminalTokenBaseClass" << endl;
        file << '{' << endl;
        file << "public:" << endl;
        file << '\t' << terminalTokens[i].string << "_Type(char* _matchedText, char* _preceedingWhitespace);" << endl;
        file << "};" << endl;
    }
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
    stringstream derivationName;
    derivationName << name << "_Type";
    rules->outputType(file,(char*)derivationName.str().c_str());
}
void DerivationRules_Rule::outputType(ofstream* file, char* typeName)
{
    (*file) << "class " << typeName << ": public NonTerminalBaseClass" << endl;
    (*file) << '{' << endl;
    (*file) << "public:" << endl;
    (*file) << '\t' << typeName << '(';
    if (rules.size() == 1)
        rules[0]->outputConstructorArguments(file);
    else
        (*file) << "const unsigned short _numargs";
    (*file) << ");" << endl;
    (*file) << "\tvirtual ~" << typeName << "();" << endl;
    if (rules.size() == 1)
        rules[0]->outputGetDeclarations(file);
    (*file) << "private:" << endl;
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
    if (terminalTokens.lookup(name) == -1)
        (*file) << name << "_Type ";
    else
        (*file) << name << "_Type ";
}
void Symbol_Rule_CHARACTER::outputType(ofstream* file)
{/*
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
    Token_Rule2* token = stringAliases[stringAliases.lookup(string)].userData;
    (*file) << token->getId() << "_Type" << ' ';
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



















