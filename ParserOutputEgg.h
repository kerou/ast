#pragma once
class BisonRules_Rule;
class UnionMembers_Rule;
class BisonDeclarations_Rule;
extern class ParserOutput
{
public:
    ParserOutput();
    void addRules(BisonRules_Rule* _rules);
    void addUnion(UnionMembers_Rule* _unionDef);
    void addDeclarations(BisonDeclarations_Rule* _declarations);
    void outputBison();
private:
    BisonRules_Rule* rules;
    UnionMembers_Rule* unionDef;
    BisonDeclarations_Rule* declarations;
}g_ParserOutput;
#include <vector>
using namespace std;

/// **** RULES SECTION ****
#include <fstream>
class Symbol_Rule
{
public:
    virtual void outputRule(std::ofstream* file)=0;
};
class Symbols_Rule
{
public:
    Symbols_Rule(void);
    Symbols_Rule(Symbol_Rule* symbol);
    void addSymbol(Symbol_Rule* symbol);
    void outputRule(std::ofstream* file);
    unsigned int ruleElements(){return symbols.size();}
private:
    vector<Symbol_Rule*> symbols;
};
class DerivationRules_Rule
{
public:
    DerivationRules_Rule(Symbols_Rule* symbols);
    void addRule(Symbols_Rule* symbols);
    void outputRule(std::ofstream* file, char* typeName);
private:
    vector<Symbols_Rule*> rules;
};
class BisonRule_Rule
{
public:
    BisonRule_Rule(char* _name, DerivationRules_Rule* _rules);
    void outputRule(std::ofstream* file);
    void outputDeclaration(std::ofstream* file);
    void outputUnionMember(std::ofstream* file);
private:
    char* name;
    DerivationRules_Rule* rules;
};
class BisonRules_Rule
{
public:
    BisonRules_Rule(BisonRule_Rule* rule);
    void addRule(BisonRule_Rule* rule);
    void outputUnionMembers(std::ofstream* file);
    void outputDeclarations(std::ofstream* file);
    void outputRules(std::ofstream* file);
private:
    vector<BisonRule_Rule*> rules;
};


/// Derived possibilities
class Symbol_Rule_ID: public Symbol_Rule
{
public:
    Symbol_Rule_ID(char* _name);
    void outputRule(std::ofstream* file);
private:
    char* name;
};
class Symbol_Rule_CHARACTER: public Symbol_Rule
{
public:
    Symbol_Rule_CHARACTER(char _character);
    void outputRule(std::ofstream* file);
private:
    char character;
};
class Symbol_Rule_STRING: public Symbol_Rule
{
public:
    Symbol_Rule_STRING(char* _string);
    void outputRule(std::ofstream* file);
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
/// **** DECLARATIONS SECTION ****
class BisonDeclaration_Rule;
class BisonDeclarations_Rule
{
public:
    BisonDeclarations_Rule(BisonDeclaration_Rule* declaration);
    void addRule(BisonDeclaration_Rule* declaration);
    void outputDeclarations(std::ofstream* file);
    void outputUnionMembers(std::ofstream* file);
    void outputAliasDeclarations(std::ofstream* file);
private:
    std::vector<BisonDeclaration_Rule*> declarations;

};
class BisonDeclaration_Rule
{
public:
    virtual void outputDeclaration(std::ofstream* file){}
    virtual void outputUnionMember(std::ofstream* file){}
    virtual void outputAliasDeclaration(std::ofstream* file){}
};
/// Bison declaration derivations
class UnionDeclaration_Rule;
class StartDeclaration_Rule;
class TokenDeclaration_Rule;
class TypeDeclaration_Rule;
class BisonDeclaration_Rule1: public BisonDeclaration_Rule
{
public:
    BisonDeclaration_Rule1(UnionDeclaration_Rule* rule){}
};
class BisonDeclaration_Rule2: public BisonDeclaration_Rule
{
public:
    BisonDeclaration_Rule2(StartDeclaration_Rule* rule){}
};
class BisonDeclaration_Rule3: public BisonDeclaration_Rule
{
public:
    BisonDeclaration_Rule3(TokenDeclaration_Rule* _rule);
private:
    void outputDeclaration(std::ofstream* file);
    void outputUnionMember(std::ofstream* file);
    void outputAliasDeclaration(std::ofstream* file);
    TokenDeclaration_Rule* rule;
};
class BisonDeclaration_Rule4: public BisonDeclaration_Rule
{
public:
    BisonDeclaration_Rule4(TypeDeclaration_Rule* rule){}
};
class UnionDeclaration_Rule
{
public:
    UnionDeclaration_Rule(UnionMembers_Rule* members);
};
class SemanticValue_Rule
{
public:
    virtual void outputDeclaration(std::ofstream* file){}
};
class SemanticValue_Rule1: public SemanticValue_Rule
{
public:
    SemanticValue_Rule1();
};
class SemanticValue_Rule2: public SemanticValue_Rule
{
public:
    SemanticValue_Rule2(char* _id);
    void outputDeclaration(std::ofstream* file){(*file)<<'<'<<id<<"> ";}
private:
    char* id;
};
class TokenList_Rule;
class TokenDeclaration_Rule
{
public:
    TokenDeclaration_Rule(SemanticValue_Rule* _value, TokenList_Rule* _list);
    void outputDeclaration(std::ofstream* file);
    void outputUnionMember(std::ofstream* file);
    void outputAliasDeclaration(std::ofstream* file);
private:
    SemanticValue_Rule* value;
    TokenList_Rule* list;
};
class Token_Rule
{
public:
    Token_Rule(char* _id);
    virtual void outputDeclaration(std::ofstream* file)=0;
    virtual void outputUnionMember(std::ofstream* file)=0;
    virtual void outputAliasDeclaration(std::ofstream* file)=0;
protected:
    char* id;
};
class TokenList_Rule
{
public:
    TokenList_Rule(Token_Rule* token);
    void addToken(Token_Rule* token);
    void outputDeclaration(std::ofstream* file);
    void outputUnionMembers(std::ofstream* file);
    void outputAliasDeclarations(std::ofstream* file);
private:
    std::vector<Token_Rule*> rules;
};
class Token_Rule1: public Token_Rule
{
public:
    Token_Rule1(char* _id);
    void outputDeclaration(std::ofstream* file);
    void outputUnionMember(std::ofstream* file);
    void outputAliasDeclaration(std::ofstream* file);
private:
};
class Token_Rule2: public Token_Rule
{
public:
    Token_Rule2(char* _id, char* _string);
    void outputDeclaration(std::ofstream* file);
    void outputUnionMember(std::ofstream* file);
    void outputAliasDeclaration(std::ofstream* file);
private:
    char* string;
};
