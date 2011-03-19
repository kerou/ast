int yyparse();
int yylex();
#include <vector>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "ParserOutputEgg.hpp"

using namespace std;
extern FILE* yyin;
extern char* yytext;
bool error = false;
std::string resourceDirectory;
std::string outputfile;
int main(int argv, char* argc[])
{
  if (argv != 4)
    {
      return -1;
    }
  yyin = fopen(argc[1],"r");
  if (yyin == NULL)
    {
      std::cout << "ast: " << argc[1] << ": No such file" << std::endl;
      return -1;
    }
  resourceDirectory = argc[2];
  outputfile = argc[3];
  yyparse();
  fclose(yyin);
  g_ParserOutput.output();
  if (!error)
    {
      std::cout << "Success" << std::endl;
      return 0;
    }
  return 1;
}
extern int line, character, lastToken, mode;
extern char* yytext;
void yyerror(char* s)
{
  error = true;
  cout << "yyerror: " << s << endl
       << "line: " << line << endl
       << "character: " << character << endl
       << "matched string: " << yytext << ' ' << endl
       << "last token: " << lastToken << endl
       << "mode: " << mode << endl;
}
