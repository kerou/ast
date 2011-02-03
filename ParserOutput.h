#pragma once

extern class ParserOutput
{
public:
    ParserOutput();
private:
}g_ParserOutput;

class Rule
{
};
class CharacterLiteral
{
};
// Declarations for Bison_Rule
class PCNT_CODE_INSERTION_Token;
class BisonDeclarations_Rule;
class PCNT_PCNT_Token;
class BisonRules_Rule;

class Bison_Rule: public Rule
{
    Bison_Rule(PCNT_CODE_INSERTION_Token*,BisonDeclarations_Rule*,PCNT_PCNT_Token*,BisonRules_Rule*,PCNT_PCNT_Token*);
};
// Declarations for BisonDeclarations_Rule
class BisonDeclaration_Rule;

class BisonDeclarations_Rule: public Rule
{
protected:
    BisonDeclarations_Rule();
};
/// BisonDeclarations_Rule 's derivation rules
class BisonDeclarations_Rule1: public BisonDeclarations_Rule
{
    BisonDeclarations_Rule1();
};
class BisonDeclarations_Rule2: public BisonDeclarations_Rule
{
    BisonDeclarations_Rule2(BisonDeclaration_Rule*);
};
class BisonDeclarations_Rule3: public BisonDeclarations_Rule
{
    BisonDeclarations_Rule3(BisonDeclarations_Rule*,BisonDeclaration_Rule*);
};
/// Declarations for BisonDeclaration_Rule
class UnionDeclaration_Rule;
class StartDeclaration_Rule;
class TokenDeclaration_Rule;
class TypeDeclaration_Rule;

class BisonDeclaration_Rule: public Rule
{
protected:

};
/// BisonDeclaration_Rule 's derivation rules
class BisonDeclaration_Rule1: public BisonDeclaration_Rule
{
public:
    BisonDeclaration_Rule1(UnionDeclaration_Rule*);
};

class BisonDeclaration_Rule2: public BisonDeclaration_Rule
{
public:
    BisonDeclaration_Rule2(StartDeclaration_Rule*);
};

class BisonDeclaration_Rule3: public BisonDeclaration_Rule
{
public:
    BisonDeclaration_Rule3(TokenDeclaration_Rule*);
};

class BisonDeclaration_Rule4: public BisonDeclaration_Rule
{
public:
    BisonDeclaration_Rule4(TypeDeclaration_Rule*);
};
/// Declarations for UnionDeclaration_Rule
class PCNT_UNION_Token;
class UnionMembersRule;



























