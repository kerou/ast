%{
#include "ParserOutputEgg.h"
int yylex();
void yyerror(char*);
%}
%union
{
  char* string;
  char character;
  BisonRules_Rule* bisonRules;
  BisonRule_Rule* bisonRule;
  DerivationRules_Rule* derivationRules;
  Symbols_Rule* symbols;
  Symbol_Rule* symbol;
}
%token <string> ID STRING CODE_INSERTION PCNT_CODE_INSERTION
%token <character> CHARACTER
%token PCNT_PCNT "%%"
	PCNT_TOKEN "%token"
	PCNT_TYPE "%type"
	PCNT_UNION "%union"
	PCNT_START "%start"
	
//	%type <string> BisonDeclaration	BisonDeclarations
%type <bisonRules> BisonRules
%type <bisonRule> BisonRule
%type <derivationRules> DerivationRules
%type <symbols> Symbols
%type <symbol> Symbol
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
	"%union" CODE_INSERTION //'{' UnionMembers '}'
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
	BisonRule {$$ = new BisonRules_Rule($1);}
    |	BisonRules BisonRule {$$ = $1; $$->addRule($2);}
    ;
BisonRule:
	ID ':' DerivationRules ';' {$$ = new BisonRule_Rule($1,$3);}
    ;
DerivationRules:
    	Symbols CodeInsertion {$$ = new DerivationRules_Rule($1);}
    |	DerivationRules '|' Symbols CodeInsertion{$$ = $1; $$->addRule($3);}
    ;
CodeInsertion:
	/* Empty line */
    |	PCNT_CODE_INSERTION
    ;
Symbols:
	/* Empty line */ {$$ = new Symbols_Rule;}
    |	Symbol {$$ = new Symbols_Rule($1);}
    |	Symbols Symbol {$$ = $1; $$->addSymbol($2);}
    ;
Symbol:
	ID {$$ = new Symbol_Rule_ID($1);}
    |	CHARACTER {$$ = new Symbol_Rule_CHARACTER($1);}
    |	STRING {$$ = new Symbol_Rule_STRING($1);}
    ;
%%



