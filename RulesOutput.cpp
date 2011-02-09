#include "ParserOutputEgg.h"
#include "SymbolTable.h"
SymbolTable characterLiterals;
SymbolTable terminalTokens;
#include <cstring>
#include <fstream>
using namespace std;
const char preamble[] = "%{\n\
\t#include \"TestOutput.h\"\n\
\tint yylex();\n\
%}\n";
void outputCharacterLiteralAliasDeclarations(ofstream* file)
{
    for (int i = 0; i < characterLiterals.size(); i++)
    {
        char literal = characterLiterals[i][0];
        int literalInt = literal;
        (*file) << "%type <character> LitChar" << literalInt << " \""  << literal << "\"\n";
    }
}
void outputCharacterLiteralAliasRules(ofstream* file)
{
    for (int i = 0; i < characterLiterals.size(); i++)
    {
        char literal = characterLiterals[i][0];
        int literalInt = literal;
        (*file) << "LitChar" << literalInt << ":\n\t '" << literal << "' {$$ = '" << literal << "';}\n;\n";
    }
}
void outputAliasRules(ofstream* file)
{
    for (int i = 0; i < terminalTokens.size(); i++)
    {
        (*file) << terminalTokens[i] << "_Alias:\n\t" << terminalTokens[i] << "\n;\n";
    }
}
void ParserOutput::outputBison()
{
    std::ofstream file("TestOutput.y");
    file << preamble;

    file << "%union\n{\n";
    rules->outputUnionMembers(&file);
    file << endl;
    declarations->outputUnionMembers(&file);
    file << '}' << endl;

    rules->outputDeclarations(&file);
    file << endl;

    declarations->outputDeclarations(&file);
    declarations->outputAliasDeclarations(&file);
    outputCharacterLiteralAliasDeclarations(&file);
    file << "%%" << endl;
    rules->outputRules(&file);
    outputAliasRules(&file);
    outputCharacterLiteralAliasRules(&file);
    file << "%%" << endl;
}
void BisonDeclarations_Rule::outputDeclarations(ofstream* file)
{
    for (unsigned int i = 0; i < declarations.size(); i++)
    {
        declarations[i]->outputDeclaration(file);
    }
}
void BisonDeclarations_Rule::outputUnionMembers(ofstream* file)
{
    for (unsigned int i = 0; i < declarations.size(); i++)
    {
        declarations[i]->outputUnionMember(file);
    }
}
void BisonDeclarations_Rule::outputAliasDeclarations(ofstream* file)
{
    for (unsigned int i = 0; i < declarations.size(); i++)
    {
        declarations[i]->outputAliasDeclaration(file);
    }
}
BisonDeclaration_Rule3::BisonDeclaration_Rule3(TokenDeclaration_Rule* _rule)
{
    rule = _rule;
}
void BisonDeclaration_Rule3::outputDeclaration(ofstream* file)
{
    rule->outputDeclaration(file);
}
void BisonDeclaration_Rule3::outputUnionMember(ofstream* file)
{
    rule->outputUnionMember(file);
}
void BisonDeclaration_Rule3::outputAliasDeclaration(ofstream* file)
{
    rule->outputAliasDeclaration(file);
}
void TokenDeclaration_Rule::outputDeclaration(ofstream* file)
{
    (*file) << "%token ";
    value->outputDeclaration(file);
    (*file) << '\n';
    list->outputDeclaration(file);
}
void TokenDeclaration_Rule::outputUnionMember(ofstream* file)
{
    list->outputUnionMembers(file);
}
void TokenDeclaration_Rule::outputAliasDeclaration(ofstream* file)
{
    list->outputAliasDeclarations(file);
}
void TokenList_Rule::outputDeclaration(ofstream* file)
{
    for (unsigned int i = 0; i < rules.size(); i++)
    {
        rules[i]->outputDeclaration(file);
    }
}
void TokenList_Rule::outputUnionMembers(ofstream* file)
{
    for (unsigned int i = 0; i < rules.size(); i++)
    {
        rules[i]->outputUnionMember(file);
    }
}
void TokenList_Rule::outputAliasDeclarations(ofstream* file)
{
    for (unsigned int i = 0; i < rules.size(); i++)
    {
        rules[i]->outputAliasDeclaration(file);
    }
}
Token_Rule::Token_Rule(char* _id)
{
    id = _id;
    terminalTokens.add(id);
}
void Token_Rule1::outputDeclaration(ofstream* file)
{
    (*file) << '\t' << id << '\n';
}
void Token_Rule1::outputUnionMember(ofstream* file)
{
    (*file) << '\t' << id << "_Token* " << id << "_Token_Return;\n";
}
void Token_Rule1::outputAliasDeclaration(ofstream* file)
{
    (*file) << "%type <" << id << "_Token_Return> " << id << "_Alias\n";
}
void Token_Rule2::outputDeclaration(ofstream* file)
{
    (*file) << '\t' << id << '\n';//'" \"" << string << "\"\n";
}
void Token_Rule2::outputUnionMember(ofstream* file)
{
    (*file) << '\t' << id << "_Token* " << id << "_Token_Return;\n";
}
void Token_Rule2::outputAliasDeclaration(ofstream* file)
{
    (*file) << "%type <" << id  << "_Token_Return> " << id << "_Alias \"" << string << "\"\n";
}
void BisonRules_Rule::outputUnionMembers(ofstream* file)
{
    for (unsigned int i = 0; i < rules.size(); i++)
    {
        rules[i]->outputUnionMember(file);
    }
}
void BisonRules_Rule::outputDeclarations(ofstream* file)
{
    for (unsigned int i = 0; i < rules.size(); i++)
    {
        rules[i]->outputDeclaration(file);
    }
}
void BisonRules_Rule::outputRules(ofstream* file)
{
    for (unsigned int i = 0; i < rules.size(); i++)
    {
        rules[i]->outputRule(file);
        (*file) << endl << ';' << endl;
    }
}
void BisonRule_Rule::outputRule(ofstream* file)
{
    unsigned int length = strlen(name);
    char typeName[length + strlen("_Rule")];
    strcpy(typeName,name);
    strcpy(typeName+length,"_Rule");
    (*file) << name << ':' << endl;
    rules->outputRule(file,typeName);
}
void DerivationRules_Rule::outputRule(ofstream* file, char* typeName)
{
    //(*file) << '\t';
    //rules[0]->outputRule(file);
    //(*file) << "{$$ = new " << typeName << "0();}";
    for (unsigned int i = 0; i < rules.size(); i++)
    {
        if (i == 0)
            (*file) << '\t';
        else
            (*file) << endl << "|\t";
        rules[i]->outputRule(file);
        (*file) << "{$$ = new " << typeName << i << "(";
        for (unsigned int ii = 0; ii < rules[i]->ruleElements(); ii++)
        {
            (*file) << '$' << ii+1;
            if (ii < rules[i]->ruleElements()-1)
                (*file) << ',';
        }
        (*file) << ");}";
    }
}
void Symbols_Rule::outputRule(ofstream* file)
{
    if (symbols.size() == 0)
    {
        (*file) << "/* Empty rule */";
    }
    else
        for (unsigned int i = 0; i < symbols.size(); i++)
        {
            symbols[i]->outputRule(file);
        }
}
void Symbol_Rule_ID::outputRule(ofstream* file)
{
    if (terminalTokens.lookup(name) == -1)
        (*file) << name << ' ';
    else
        (*file) << name << "_Alias ";
}
void Symbol_Rule_CHARACTER::outputRule(ofstream* file)
{
    (*file) << '"' << character << '"' << ' ';
}
void Symbol_Rule_STRING::outputRule(ofstream* file)
{
    (*file) << '"' << string << '"' << ' ';
}

BisonDeclarations_Rule::BisonDeclarations_Rule(BisonDeclaration_Rule* declaration)
{
    if (declaration != NULL)
    {
        declarations.push_back(declaration);
    }
}
void BisonDeclarations_Rule::addRule(BisonDeclaration_Rule* declaration)
{
    declarations.push_back(declaration);
}
/// Declarations output
void BisonRule_Rule::outputDeclaration(ofstream* file)
{
    (*file) << "%type <" << name << "_Return> " << name << endl;
}
void BisonRule_Rule::outputUnionMember(ofstream* file)
{
    (*file) << '\t' << name << "_Rule* " << name << "_Return;" << endl;
}
TokenDeclaration_Rule::TokenDeclaration_Rule(SemanticValue_Rule* _value, TokenList_Rule* _list)
{
    value = _value;
    list = _list;
}
SemanticValue_Rule1::SemanticValue_Rule1()
{
}
SemanticValue_Rule2::SemanticValue_Rule2(char* _id)
{
    id = _id;
}
TokenList_Rule::TokenList_Rule(Token_Rule* token)
{
    rules.push_back(token);
}
void TokenList_Rule::addToken(Token_Rule* token)
{
    rules.push_back(token);
}
Token_Rule1::Token_Rule1(char* _id)
:Token_Rule(_id)
{
}
Token_Rule2::Token_Rule2(char* _id, char* _string)
:Token_Rule(_id)
{
    string = _string;
}
Symbol_Rule_CHARACTER::Symbol_Rule_CHARACTER(char _character)
{
    character = _character;
    characterLiterals.add(&character);
}
Symbol_Rule_ID::Symbol_Rule_ID(char* _name)
{
    name = _name;
}
