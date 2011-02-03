%{
#include "ParserOutput.h"
int yylex();
void yyerror(char*);
%}
%union
{
  char* string;
  char character;
}
%token <string> ID STRING CODE_INSERTION PCNT_CODE_INSERTION
%token <character> CHARACTER
%token PCNT_PCNT "%%"
	PCNT_TOKEN "%token"
	PCNT_TYPE "%type"
	PCNT_UNION "%union"
	PCNT_START "%start"
	
%type <string> BisonDeclaration	BisonDeclarations
%%
//;

Bison:
	PCNT_CODE_INSERTION BisonDeclarations "%%" BisonRules "%%"
    ;
/// Declaration section
BisonDeclarations:
	/* Empty line */
    |	BisonDeclaration
    |	BisonDeclarations BisonDeclaration
    ;

BisonDeclaration:
	UnionDeclaration
    |	StartDeclaration
    |	TokenDeclaration
    |	TypeDeclaration
    ;

UnionDeclaration:
	"%union" '{' UnionMembers '}'
    ;
UnionMembers:
	/* Empty line */
    |	UnionMember
    |	UnionMembers UnionMember
    ;
UnionMember:
	ID ID ';'
    |	ID '*' ID ';'
    ;
StartDeclaration:
	"%start" ID
    ;
TokenDeclaration:
	"%token" SemanticValue TokenList
    ;
TypeDeclaration:
	"%type" SemanticValue TypeList
    ;
SemanticValue:
	/* Empty line */
    |	'<' ID '>'
    ;
TypeList:
	Type
    |	TypeList Type
    ;
Type:
	ID
    ;
TokenList:
	Token
    |	TokenList Token
    ;
Token:
	ID
    |	ID STRING
    ;
/// Rules section
BisonRules:
	BisonRule
    |	BisonRules BisonRule
    ;
BisonRule:
	ID ':' DerivationRules ';'
    ;
DerivationRules:
    	Symbols
    |	DerivationRules '|' Symbols
    ;
Symbols:
	/* Empty line */
    |	Symbol
    |	Symbols Symbol
    ;
Symbol:
	ID
    |	CHARACTER
    |	STRING
    ;
%%



