int yyparse();
int yylex();
#include <vector>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "ParserOutputEgg.h"

using namespace std;
extern FILE* yyin;
extern char* yytext;
bool error = false;
const char* resourceDirectory;
int main(int argv, char* argc[])
{
  if (argv < 3)
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
    yyparse();
    fclose(yyin);
    g_ParserOutput.output();
    if (!error)
    {
        int boolean;
        if (false && boolean == 1)
        {
            if (system("bison -d -o TestOutput.tab.cpp TestOutput.y")==0)
            if (system("flex -oTestOutput.yy.cpp TestOutput.l")==0)
            if (system("g++ -c -std=c++0x TestOutput.tab.cpp")==0)
            if (system("g++ -c -std=c++0x TestOutput.yy.cpp")==0)
            if (system("g++ -c -std=c++0x TestOutput.cpp")==0)
            if (system("g++ TestOutput.o TestOutput.yy.o TestOutput.tab.o")==0)
            if (system("./a.out")==0)
            return 0;
        }
    }
    std::cout << "Success" << std::endl;
    return 0;
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
