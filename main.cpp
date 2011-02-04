int yyparse();
#include <vector>
#include <iostream>
#include <cstdio>
using namespace std;
extern FILE* yyin;
int main(int argv, char* argc[])
{
  yyin = fopen("Parser.y","r");
  yyparse();
  cout << "Success" << endl;
  fclose(yyin);
  return 0;
}
extern int line, character, lastToken, mode;
extern char* yytext;
void yyerror(char* s)
{
  cout << "yyerror: " << s << endl
       << "line: " << line << endl
       << "character: " << character << endl
       << "matched string: " << yytext << (int)yytext[0] << endl
       << "last token: " << lastToken << endl
       << "mode: " << mode << endl;
}
