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
  
  // Get the last position of '/'
  std::string aux(argc[0]);

  // get '/' or '\\' depending on unix/mac or windows.
#if defined(_WIN32) || defined(WIN32)
  int pos = aux.rfind('\\');
#else
  int pos = aux.rfind('/');
#endif

  // Get the path and the name
  resourceDirectory = aux.substr(0,pos+1);
  //resourceDirectory = "~/
  std::cout << "Resource dir: " << resourceDirectory << std::endl;
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
