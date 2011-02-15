#include "ParserOutputEgg.h"
#include "SymbolTable.h"
#define ALIASING
#include <sstream>
using namespace std;
extern SymbolTable<void*> characterLiterals;
extern SymbolTable<Token_Rule*> terminalTokens;
extern SymbolTable<Symbol_Rule_List*> listRules;
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
        (*file) << "class " << listRules[i] << "_List" << endl;
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
void ParserOutput::outputHeader()
{
    ofstream file("TestOutput.h");

    file << "#pragma once" << endl;
    /// Forward declarations
    file << "/// Forward declarations of all types" << endl;
    rules->outputTypeDeclarations(&file);
    file << "/// List rule types" << endl;
    outputListTypeDeclarations(&file);
    file << "/// Terminal token aliases" << endl;
    for (int i = 0; i < terminalTokens.size(); i++)
    {
        file << "class " << terminalTokens[i].string << "_Type;" << endl;
    }
    file << "/// Character literal aliases" << endl;
    for (int i = 0; i < characterLiterals.size(); i++)
    {
        int literalInt = characterLiterals[i][0];
        file << "class LitChar" << literalInt << ';' << endl;
    }
    /// Type definitions
    file << "/// Actual declarations" << endl;
    file << "/// User defined rules" << endl;
    rules->outputTypes(&file);
    file << "/// List rules types" << endl;
    file << "#include <vector>" << endl;
    outputListTypes(&file);
    for (int i = 0; i < terminalTokens.size(); i++)
    {
        file << "class " << terminalTokens[i].string << "_Type" << endl;
        file << '{' << endl;
        file << "public:" << endl;
        file << '\t' << terminalTokens[i].string << "_Type(";
        terminalTokens[i].userData->outputConstructorArguments(&file);
        file << ");" << endl;
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
    (*file) << "class " << typeName << endl;
    (*file) << '{' << endl;
    (*file) << "public:" << endl;
    (*file) << '\t' << typeName << '(';
    if (rules.size() == 1)
        rules[0]->outputConstructorArguments(file);
    (*file) << ");" << endl;
    (*file) << "\tvirtual ~" << typeName << "();" << endl;
    rules[0]->outputGetDeclarations(file);
    (*file) << "private:" << endl;
    rules[0]->outputMemberDeclarations(file);
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
            rules[i]->outputMemberDeclarations(file);
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
void Symbols_Rule::outputMemberDeclarations(ofstream* file)
{
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
        (*file) << "* getArg" << i+1 << "(){return arg" << i+1 << ";}" << endl;
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
    (*file) << "char ";
}
void Symbol_Rule_STRING::outputType(ofstream* file)
{
    (*file) << '"' << string << '"' << ' ';
}
void Symbol_Rule_List::outputType(ofstream* file)
{
    (*file) << name << "_List ";
}
void Token_Rule1::outputConstructorArguments(ofstream* file)
{
    UnionMember_Rule* member = g_ParserOutput.getUnion()->getMember(semanticValue->getString());
    (*file) << member->getTypeName();
}
void Token_Rule2::outputConstructorArguments(ofstream* file)
{
    UnionMember_Rule* member = g_ParserOutput.getUnion()->getMember(semanticValue->getString());
    if (member != NULL)
        (*file) << member->getTypeName();
}
UnionMembers_Rule* ParserOutput::getUnion()
{
    return unionDef;
}
#include <cstring>
UnionMember_Rule* UnionMembers_Rule::getMember(char* id)
{
    if (id != NULL)
    for (unsigned int i = 0; i < members.size(); i++)
    {
        if (strcmp(members[i]->getName(),id)==0)
        {
            return members[i];
        }
    }
    return NULL;
}





















