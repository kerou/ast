#include "ParserOutputEgg.h"
#include "SymbolTable.h"
#include <sstream>
using namespace std;
extern SymbolTable<void*> characterLiterals;
extern SymbolTable<Token_Rule*> terminalTokens;
extern SymbolTable<Symbol_Rule_List*> listRules;
void outputListCode(ofstream* file)
{
    for (int i = 0; i < listRules.size(); i++)
    {
        (*file) << listRules[i] << "_List::" << listRules[i] << "_List()" << endl;
        (*file) << '{' << endl;
        (*file) << '}' << endl;
        (*file) << "void " << listRules[i] << "_List::add(" << listRules[i] << "_Token* token)" << endl;
        (*file) << '{' << endl;
        (*file) << "\tlist.push_back(token);" << endl;
        (*file) << '}' << endl;
    }
}
void ParserOutput::outputCpp()
{
    ofstream file("TestOutput.cpp");
    file << "#include \"TestOutput.h\"" << endl;
    rules->outputTypeDefinitions(&file);
    outputListCode(&file);
    for (int i = 0; i < terminalTokens.size(); i++)
    {
        file << terminalTokens[i].string << "_Token::" << terminalTokens[i].string << "_Token(";
        terminalTokens[i].userData->outputConstructorArguments(&file);
        file << ')' << endl;
        file << '{' << endl;
        file << '}' << endl;
    }
}
void BisonRules_Rule::outputTypeDefinitions(ofstream* file)
{
    for (unsigned int i = 0; i < rules.size(); i++)
    {
        rules[i]->outputTypeDefinition(file);
    }
}
void BisonRule_Rule::outputTypeDefinition(ofstream* file)
{
    stringstream derivationName;
    derivationName << name << "_Rule";
    rules->outputTypeDefinition(file,(char*)derivationName.str().c_str());
}
void DerivationRules_Rule::outputTypeDefinition(ofstream* file, char* typeName)
{
    /// Constructor
    (*file) << typeName << "::" << typeName << '(';
    if (rules.size() == 1)
        rules[0]->outputConstructorArguments(file);
    (*file) << ')' << endl;
    (*file) << '{' << endl;
    if (rules.size() == 1)
        rules[0]->outputConstructorCode(file);
    (*file) << '}' << endl;
    if (rules.size() != 1)
    {
        for (unsigned int i = 0; i < rules.size(); i++)
        {
            (*file) << typeName << i+1 << "::" << typeName << i+1 << '(';
            rules[i]->outputConstructorArguments(file);
            (*file) << ')' << endl;
            (*file) << '{' << endl;
            rules[i]->outputConstructorCode(file);
            (*file) << '}' << endl;
        }
    }
    /// Destructor
    (*file) << typeName << "::~" << typeName << "()" << endl;
    (*file) << '{' << endl;
    if (rules.size() == 1)
        rules[0]->outputDestructorCode(file);
    (*file) << '}' << endl;
    if (rules.size() != 1)
    {
        for (unsigned int i = 0; i < rules.size(); i++)
        {
            (*file) << typeName << i+1 << "::~" << typeName << i+1 << "()" << endl;
            (*file) << '{' << endl;
            rules[i]->outputDestructorCode(file);
            (*file) << '}' << endl;
        }
    }
}
void Symbols_Rule::outputConstructorCode(ofstream* file)
{
    for (unsigned int i = 0; i < symbols.size(); i++)
    {
        (*file) << "\targ" << i+1 << " = _arg" << i+1 << ';' << endl;
    }
}

void Symbols_Rule::outputDestructorCode(ofstream* file)
{
    for (unsigned int i = 0; i < symbols.size(); i++)
    {
        (*file) << "\tdelete arg" << i+1 << ';' << endl;
    }
}























