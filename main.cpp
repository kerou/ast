int yyparse();
#include <cstdio>
extern FILE* yyin;
int main(int argv, char* argc[])
{
  yyin = fopen("Parser.y","r");
  yyparse();
  return 0;
}
#include <vector>
#include <iostream>
using namespace std;
extern int line, character, lastToken;
struct Token
    {
	Token(int t,char* c){token = t;match = c;}
	int token;
	string match;
    };
extern vector<Token> tokens;
extern char* yytext;
void yyerror(char* s)
{
  cout << "yyerror: " << s << endl
       << "line: " << line << endl
       << "character: " << character << endl
       << "matched string: " << yytext << (int)yytext[0] << endl
       << "last tokens: " << endl;
  for (int i = 1; i < 7; i++)
    {
      cout << tokens[tokens.size()-i].token << endl << tokens[tokens.size()-i].match << endl;
    }
}
