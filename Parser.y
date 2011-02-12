%{
#include "ParserOutputEgg.h"
int yylex();
void yyerror(char*);
void changeMode();
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
  UnionMembers_Rule* unionMembers;
  UnionMember_Rule* unionMember;

  BisonDeclarations_Rule* bisonDeclarations;
  BisonDeclaration_Rule* bisonDeclaration;

  UnionDeclaration_Rule* unionDeclaration;
  StartDeclaration_Rule* startDeclaration;
  TokenDeclaration_Rule* tokenDeclaration;
  SemanticValue_Rule* semanticValue;
  TokenList_Rule* tokenList;
  Token_Rule* token;
  TypeDeclaration_Rule* typeDeclaration;


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
%type <unionMembers> UnionMembers
%type <unionMember> UnionMember
%type <bisonDeclarations> BisonDeclarations
%type <bisonDeclaration> BisonDeclaration
%type <unionDeclaration> UnionDeclaration
%type <startDeclaration> StartDeclaration
%type <tokenDeclaration> TokenDeclaration
%type <semanticValue> SemanticValue
%type <tokenList> TokenList
%type <token> Token
%type <typeDeclaration> TypeDeclaration
%%
//;

Script:
PCNT_CODE_INSERTION BisonDeclarations PCNT_PCNT BisonRules PCNT_PCNT {g_ParserOutput.addDeclarations($2);g_ParserOutput.addRules($4);g_ParserOutput.output();}
    ;
/// Declaration section
BisonDeclarations:
	/* Empty line */ {$$ = new BisonDeclarations_Rule(NULL);}
    |	BisonDeclaration {$$ = new BisonDeclarations_Rule($1);}
    |	BisonDeclarations BisonDeclaration {$$ = $1; $$->addRule($2);}
    ;

BisonDeclaration:
	UnionDeclaration {$$ = new BisonDeclaration_Rule1($1); changeMode();}
    |	StartDeclaration {$$ = new BisonDeclaration_Rule2($1);}
    |	TokenDeclaration {$$ = new BisonDeclaration_Rule3($1);}
    |	TypeDeclaration {$$ = new BisonDeclaration_Rule4($1);}
    ;

UnionDeclaration:
	PCNT_UNION '{' UnionMembers '}' {g_ParserOutput.addUnion($3);}
    ;
UnionMembers:
	/* Empty line */ {$$ = new UnionMembers_Rule();}
    |	UnionMember {$$ = new UnionMembers_Rule($1);}
    |	UnionMembers UnionMember {$$ = $1; $$->addMember($2);}
    ;
UnionMember:
	ID ID ';' {$$ = new UnionMember_Rule1($1,$2);}
    |	ID '*' ID ';' {$$ = new UnionMember_Rule2($1,$3);}
    ;
StartDeclaration:
	PCNT_START ID {$$ = NULL;}
    ;
TokenDeclaration:
	PCNT_TOKEN SemanticValue TokenList {$$ = new TokenDeclaration_Rule($2,$3);$3->setSemanticValue($2);}
    ;
TypeDeclaration:
	PCNT_TYPE SemanticValue TypeList {$$ = NULL;}
    ;
SemanticValue:
	/* Empty line */ {$$ = new SemanticValue_Rule1();}
    |	'<' ID '>' {$$ = new SemanticValue_Rule2($2);}
    ;
TypeList:
	Type
    |	TypeList Type
    ;
Type:
	ID
    ;
TokenList:
	Token {$$ = new TokenList_Rule($1);}
    |	TokenList Token {$$ = $1; $$->addToken($2);}
    ;
Token:
	ID {$$ = new Token_Rule1($1);}
|	ID STRING {$$ = new Token_Rule2($1,$2);}
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
    |	DerivationRules '|' Symbols CodeInsertion {$$ = $1; $$->addRule($3);}
    ;
CodeInsertion:
	/* Empty line */
    |	CODE_INSERTION
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



