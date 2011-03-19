#include "ParserOutputEgg.h"
#include "SymbolTable.h"
#include <sstream>
using namespace std;
extern std::unordered_map<std::string,void*> characterLiterals;
extern std::unordered_map<std::string,Token_Rule*> terminalTokens;
extern std::unordered_map<std::string,Symbol_Rule_List*> listRules;
void outputListCode(ofstream* file)
{
    auto iter = listRules.begin();
    while (iter != listRules.end())
    {
        (*file) << iter->first << "_List::" << iter->first << "_List()" << endl;
        (*file) << '{' << endl;
        (*file) << '}' << endl;
        (*file) << "void " << iter->first << "_List::add(" << iter->first << "_Type* token)" << endl;
        (*file) << '{' << endl;
        (*file) << "\tlist.push_back(token);" << endl;
        (*file) << '}' << endl;
		iter++;
    }
}
void outputFile(ofstream* outfile, const char* infileName);
void ParserOutput::outputCpp()
{
    ofstream file("TestOutput.cpp");
    std::string fullFilename(resourceDirectory);
    fullFilename = fullFilename + "astres/SourcePreamble.txt";
    outputFile(&file,fullFilename.c_str());
    rules->outputTypeDefinitions(&file);
    outputListCode(&file);
    auto iter = terminalTokens.begin();
    while (iter != terminalTokens.end())
    {
        file << iter->first << "_Type::" << iter->first << "_Type(char* _matchedText, char* _preceedingWhitespace)" << endl;
        file << ":TerminalTokenBaseClass(_matchedText,_preceedingWhitespace)" << endl;
        file << '{' << endl;
        file << '}' << endl;
		iter++;
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
    derivationName << name << "_Type";
    rules->outputTypeDefinition(file,(char*)derivationName.str().c_str());
}
void DerivationRules_Rule::outputTypeDefinition(ofstream* file, char* typeName)
{
    /// Constructor
    (*file) << typeName << "::" << typeName << '(';
    if (rules.size() == 1)
        rules[0]->outputConstructorArguments(file);
    else
        (*file) << "const unsigned short _numargs";
    (*file) << ')' << endl;
    if (rules.size() == 1)
        (*file) << ":NonTerminalBaseClass(" << rules[0]->size() << ')' << endl;
    else
        (*file) << ":NonTerminalBaseClass(_numargs)" << endl;
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
            (*file) << ':' << typeName << '(' << rules[i]->size() << ')' << endl;
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
    (*file) << "\targs = new RuleTypeBaseClass*[numargs];" << endl;
    for (unsigned int i = 0; i < symbols.size(); i++)
    {
        (*file) << "\targs[" << i << "] = _arg" << i+1 << ';' << endl;
    }
}

void Symbols_Rule::outputDestructorCode(ofstream* file)
{
    for (unsigned int i = 0; i < symbols.size(); i++)
    {
        (*file) << "\tdelete args[" << i << "];" << endl;
    }
}























