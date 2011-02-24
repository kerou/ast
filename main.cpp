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
int main(int argv, char* argc[])
{
    yyin = fopen("TestInput.y","r");
    yyparse();
    fclose(yyin);
    g_ParserOutput.output();
    if (!error)
    {
        int boolean;
        cin >> boolean;
        if (boolean == 1)
        {
            if (system("bison -d -o TestOutput.tab.cpp TestOutput.y")==0)
            if (system("flex -oTestOutput.yy.cpp TestOutput.l")==0)
            if (system("g++ -c -std=c++0x TestOutput.tab.cpp")==0)
            if (system("g++ -c -std=c++0x TestOutput.yy.cpp")==0)
            if (system("g++ -c -std=c++0x TestOutput.cpp")==0)
            if (system("g++ TestOutput.o TestOutput.yy.o TestOutput.tab.o")==0)
            if (system("a.exe")==0)
            return 0;
        }
    }
    return -1;
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
