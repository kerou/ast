%{
#include "ParserOutputEgg.hpp"
#include <vector>
int yylex();
void yyerror(char*);
void changeMode();
void lexRulesMode();
void regexMode();
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

  BisonDeclarations_Rule* bisonDeclarations;
  BisonDeclaration_Rule* bisonDeclaration;

  SymTableDeclaration_Rule* symTableDeclaration;
  SymTableInput_Rule* symTableInput;
  SymTableOutput_Rule* symTableOutput;
  SymTableNotEqual_Rule* symTableNotEqual;
  StartDeclaration_Rule* startDeclaration;
  TokenDeclaration_Rule* tokenDeclaration;
  TokenList_Rule* tokenList;
  Token_Rule* token;
  TypeDeclaration_Rule* typeDeclaration;

  std::vector<char*>* typeList;

  LexModes_Rule* lexModes;
  LexMode_Rule* lexMode;
  LexRules_Rule* lexRules;
  LexRule_Rule* lexRule;
  LexCommand_Rule* lexCommand;
}
%debug
%type <lexCommand> ':'
%token <string> ID STRING CODE_INSERTION PCNT_CODE_INSERTION REGULAR_EXPRESSION
%token <character> CHARACTER
%token PCNT_PCNT "%%"
	PCNT_TOKEN "%token"
	PCNT_TYPE "%type"
	PCNT_UNION "%union"
	PCNT_START "%start"
	PCNT_SYM_TABLE "%symtable"
	LEFT_SHIFT "<<"
	RIGHT_SHIFT ">>"
	NOT_EQUAL "!="

//	%type <string> BisonDeclaration	BisonDeclarations
%type <bisonRules> BisonRules
%type <bisonRule> BisonRule
%type <derivationRules> DerivationRules
%type <symbols> Symbols
%type <symbol> Symbol
%type <bisonDeclarations> BisonDeclarations
%type <symTableDeclaration> SymTableDeclaration
%type <symTableInput> SymTableInput
%type <symTableOutput> SymTableOutput
%type <symTableNotEqual> SymTableNotEqual
%type <bisonDeclaration> BisonDeclaration
%type <startDeclaration> StartDeclaration
%type <tokenDeclaration> TokenDeclaration
%type <tokenList> TokenList
%type <typeList> TypeList
%type <token> Token
%type <typeDeclaration> TypeDeclaration
%type <lexModes> LexModes
%type <lexMode> LexMode
%type <lexRules> LexRules
%type <lexRule> LexRule
%type <lexCommand> LexCommand
%%
//;

Script:
BisonDeclarations "%%" BisonRules PCNT_PCNT {lexRulesMode();} LexModes PCNT_PCNT {g_ParserOutput.addDeclarations($1);g_ParserOutput.addRules($3);g_ParserOutput.addLexModes($6);}
    ;
/// Declaration section
BisonDeclarations:
	/* Empty line */ {$$ = new BisonDeclarations_Rule(NULL);}
    |	BisonDeclaration {$$ = new BisonDeclarations_Rule($1);}
    |	BisonDeclarations BisonDeclaration {$$ = $1; $$->addRule($2);}
    ;

BisonDeclaration:
        SymTableDeclaration {$$ = new BisonDeclaration_Rule1($1);}
    |   StartDeclaration {$$ = new BisonDeclaration_Rule2($1);}
    |	TokenDeclaration {$$ = new BisonDeclaration_Rule3($1);}
    |	TypeDeclaration {$$ = new BisonDeclaration_Rule4($1);}
    |	SymTableInput {$$ = new BisonDeclaration_Rule5($1);}
    |	SymTableOutput {$$ = new BisonDeclaration_Rule6($1);}
    |	SymTableNotEqual {$$ = new BisonDeclaration_Rule7($1);}
    ;
SymTableDeclaration:
        PCNT_SYM_TABLE '<' TypeList '>' ID {$$ = new SymTableDeclaration_Rule($3,$5);}
    ;
SymTableInput:
        ID "<<" ID {$$ = new SymTableInput_Rule($1,$3);}
    ;
SymTableOutput:
        ID ">>" ID {$$ = new SymTableOutput_Rule($1,$3);}
    ;
SymTableNotEqual:
        ID "!=" ID {$$ = new SymTableNotEqual_Rule($1,$3);}
    ;
StartDeclaration:
	PCNT_START ID {$$ = NULL;}
    ;
TokenDeclaration:
	PCNT_TOKEN TokenList {$$ = new TokenDeclaration_Rule($2);}
    ;
TypeDeclaration:
	PCNT_TYPE TypeList {$$ = NULL;}
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
    |   ID '*' {$$ = new Symbol_Rule_List($1,true);}
    //|   ID '+' {$$ = new Symbol_Rule_List($1,false);}
    ;
/// Lex section
LexModes:
	LexMode {$$ = new LexModes_Rule(); $$->add($1);}
    |	LexModes LexMode {$$ = $1; $$->add($2);}
    |	/* Empty line - delete this at some point */ {$$ = new LexModes_Rule();}
    ;
LexMode:
	ID '{' LexRules '}' {$$ = new LexMode_Rule($1,$3);}
    ;
LexRules:
	LexRule {$$ = new LexRules_Rule(); $$->add($1);}
    |	LexRules LexRule {$$ = $1; $$->add($2);}
    ;
LexRule:
	ID ':' {regexMode();} LexCommand ';' {$$ = new LexRule_Rule($1,$4);}
    ;
LexCommand:
	REGULAR_EXPRESSION {$$ = new LexCommand_Rule(); $$->add($1);}
|	LexCommand REGULAR_EXPRESSION {$$ = $1; $$->add($2);}
    ;
%%



