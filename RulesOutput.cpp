#define ALIASING
#include "ParserOutputEgg.h"
#include "SymbolTable.h"
SymbolTable<void*> characterLiterals;
SymbolTable<Token_Rule*> terminalTokens;
SymbolTable<Symbol_Rule_List*> listRules;
#include <cstring>
#include <fstream>
using namespace std;
const char preamble[] = "%{\n\
\t#include \"TestOutput.h\"\n\
\tint yylex();\n\
\tvoid yyerror(char*);\n\
%}\n";
void outputListUnionMembers(ofstream* file)
{
    for (int i = 0; i < listRules.size(); i++)
    {
        (*file) << '\t' << listRules[i] << "_List* " << listRules[i] << "_List_Return;" << endl;
    }
}
void outputListDeclarations(ofstream* file)
{
    for (int i = 0; i < listRules.size(); i++)
    {
        (*file) << "%type <" << listRules[i] << "_List_Return> " << listRules[i] << "_List" << endl;
    }
}
void outputListRules(ofstream* file)
{
    for (int i = 0; i < listRules.size(); i++)
    {
        listRules[i].userData->outputListRule(file);
    }
}
void outputCharacterLiteralAliasDeclarations(ofstream* file)
{
    for (int i = 0; i < characterLiterals.size(); i++)
    {
        char literal = characterLiterals[i][0];
        int literalInt = literal;
        (*file) << "%type <Character_Alias> LitChar" << literalInt << " \""  << literal << "\"\n";
    }
}
void outputCharacterLiteralAliasRules(ofstream* file)
{
    for (int i = 0; i < characterLiterals.size(); i++)
    {
        char literal = characterLiterals[i][0];
        int literalInt = literal;
        (*file) << "LitChar" << literalInt << ":\n\t '" << literal << "' {$$ = new char('" << literal << "');}\n;\n";
    }
}
void outputAliasRules(ofstream* file)
{
    for (int i = 0; i < terminalTokens.size(); i++)
    {
        char* semantic = terminalTokens[i].userData->getSemanticString();
        (*file) << terminalTokens[i] << "_Alias:\n\t" << terminalTokens[i];
        (*file) << " {$$ = new " << terminalTokens[i] << "_Type(";
        if (semantic != NULL)
        {
            (*file) << "$1";
        }
        (*file) << ");}\n" << ";\n";
    }
}
void ParserOutput::outputBison()
{
    ofstream file("TestOutput.y");
    /// Output %{..%}
    file << preamble;
    /// Output union declaration
    file << "%union\n{\n\t/// Union members for all of the user-defined rules\n";
    rules->outputUnionMembers(&file);
    file << "\t/// Union members for list rules\n";
    outputListUnionMembers(&file);
    file << "\t/// Union members for the terminal token aliases\n";
    declarations->outputUnionMembers(&file);
    file << "\t/// User-defined semantic values\n";
    unionDef->outputUnionMembers(&file);
    file << "\t/// Rule used for all character literal aliases\n";
    file << "\tchar* Character_Alias;" << endl;
    file << '}' << endl;

    file << "/// Setting the return types for the user-defined rules\n";
    rules->outputDeclarations(&file);
    file << "/// List rules\n";
    outputListDeclarations(&file);

    file << "/// User-defined terminal tokens\n";
    declarations->outputDeclarations(&file);
    file << "/// Terminal token aliases\n";
    declarations->outputAliasDeclarations(&file);
    file << "/// Character literal aliases\n";
    outputCharacterLiteralAliasDeclarations(&file);
    file << "%%" << endl;
    file << "/// User-defined rules\n";
    rules->outputRules(&file);
    file << "/// List rules\n";
    outputListRules(&file);
    file << "/// Terminal token alias rules\n";
    outputAliasRules(&file);
    file << "/// Character literal alias rules\n";
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
SemanticValue_Rule* lastSemantic;
Token_Rule::Token_Rule(char* _id)
{
    id = _id;
    terminalTokens.add(id,this);
    semanticValue = lastSemantic;
}
void Token_Rule1::outputDeclaration(ofstream* file)
{
    (*file) << '\t' << id << '\n';
}
void Token_Rule1::outputUnionMember(ofstream* file)
{
    (*file) << '\t' << id << "_Type* " << id << "_Type_Return;\n";
}
void Token_Rule1::outputAliasDeclaration(ofstream* file)
{
    (*file) << "%type <" << id << "_Type_Return> " << id << "_Alias\n";
}
void Token_Rule2::outputDeclaration(ofstream* file)
{
    (*file) << '\t' << id << '\n';//'" \"" << string << "\"\n";
}
void Token_Rule2::outputUnionMember(ofstream* file)
{
    (*file) << '\t' << id << "_Type* " << id << "_Type_Return;\n";
}
void Token_Rule2::outputAliasDeclaration(ofstream* file)
{
    (*file) << "%type <" << id  << "_Type_Return> " << id << "_Alias \"" << string << "\"\n";
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
    char typeName[length + strlen("_Type")];
    strcpy(typeName,name);
    strcpy(typeName+length,"_Type");
    (*file) << name << ':' << endl;
    rules->outputRule(file,typeName);
}
void DerivationRules_Rule::outputRule(ofstream* file, char* typeName)
{
    for (unsigned int i = 0; i < rules.size(); i++)
    {
        if (i == 0)
            (*file) << '\t';
        else
            (*file) << endl << "|\t";
        rules[i]->outputRule(file);
        (*file) << "{$$ = new " << typeName;
        if (rules.size() != 1) (*file) << i+1;
        (*file) << "(";
        for (unsigned int ii = 0; ii < rules[i]->size(); ii++)
        {
            (*file) << '$' << ii+1;
            if (ii < rules[i]->size()-1)
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
#ifndef ALIASING
    (*file) << '"' << character << '"' << ' ';
#else
    int literalInt = character;
    (*file) << "LitChar" << literalInt << ' ';
#endif
}
void Symbol_Rule_STRING::outputRule(ofstream* file)
{
    (*file) << '"' << string << '"' << ' ';
}
void Symbol_Rule_List::outputRule(ofstream* file)
{
    (*file) << name << "_List ";
}
void Symbol_Rule_List::outputListRule(ofstream* file)
{
    if (terminalTokens.lookup(name) == -1)
    {
        (*file) << name << "_List:" << endl;
        (*file) << '\t' << name  << " {$$ = new " << name << "_List(); $$->add($1);}" << endl;
        (*file) << "|\t" << name << "_List " << name  << " {$$ = $1; $$->add($2);}" << endl;
        (*file) << ';' << endl;
    }
    else
    {
        (*file) << name << "_List:" << endl;
        (*file) << '\t' << name << "_Alias" << " {$$ = new " << name << "_List(); $$->add($1);}" << endl;
        (*file) << "|\t" << name << "_List " << name << "_Alias" << " {$$ = $1; $$->add($2);}" << endl;
        (*file) << ';' << endl;
    }
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
    (*file) << '\t' << name << "_Type* " << name << "_Return;" << endl;
}
TokenDeclaration_Rule::TokenDeclaration_Rule(SemanticValue_Rule* _value, TokenList_Rule* _list)
{
    value = _value;
    list = _list;
}
SemanticValue_Rule1::SemanticValue_Rule1()
{
    lastSemantic = this;
}
SemanticValue_Rule2::SemanticValue_Rule2(char* _id)
{
    id = _id;
    lastSemantic = this;
    UnionMembers_Rule* members = g_ParserOutput.getUnion();
    member = members->getMember(id);
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
Symbol_Rule_List::Symbol_Rule_List(char* _name)
{
    name = _name;
    listRules.add(name,this);
}
void UnionMembers_Rule::outputUnionMembers(ofstream* file)
{
    for (unsigned int i = 0; i < members.size(); i++)
    {
        members[i]->outputUnionMember(file);
    }
}
void UnionMember_Rule1::outputUnionMember(ofstream* file)
{
    (*file) << '\t' << typeName << ' ' << name << ';' << endl;
}
void UnionMember_Rule2::outputUnionMember(ofstream* file)
{
    (*file) << '\t' << typeName << ' ' << name << ';' << endl;
}




























