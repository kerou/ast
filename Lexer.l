%{

#include <cstring>
#include <cstdio>
#define fileno(__F) ((__F)->_file)
#include "ParserOutputEgg.h"
#include "Parser.tab.hpp"
#include <string>
#include <vector>
#include <iostream>
#define COPY_STRING_BUFFER yylval.string = new char[stringBuffer.size()]; strcpy(yylval.string,stringBuffer.c_str());

#define COUNT character += yyleng;
#define LINE line++; character = 1;
#define RETURN(x) lastToken  = x; return x;
    using namespace std;
    string stringBuffer;
    int line = 1;
    int character = 1;
    int lastToken = -1;
    int mode = INITIAL;

    int codeInsertionType = -1; // CODE_INSERTION or PCNT_CODE_INSERTION, or -1 for neither

extern "C"
{
    int yywrap()
    {
	return 1;
    }
}

%}
%x STRING_STATE
%x CODE_INSERTION_STATE
%x C_COMMENT_STATE
%x REGEX_MODE
%x REGEX_MODE_CHARACTER
%s POST_UNION_STATE
%s LEX_MODES_STATE
%%

						      /*
							Code insertion parsing
						     */
<INITIAL>"%{" BEGIN(CODE_INSERTION_STATE); codeInsertionType = PCNT_CODE_INSERTION; stringBuffer.clear();COUNT
<POST_UNION_STATE>\{ BEGIN(CODE_INSERTION_STATE); codeInsertionType = CODE_INSERTION; stringBuffer.clear();COUNT
<CODE_INSERTION_STATE>"%}" BEGIN(mode); COPY_STRING_BUFFER;COUNT RETURN(codeInsertionType);
<CODE_INSERTION_STATE>\} BEGIN(mode); COPY_STRING_BUFFER;COUNT RETURN(codeInsertionType);
<CODE_INSERTION_STATE>. stringBuffer.push_back(yytext[0]);COUNT
<CODE_INSERTION_STATE>\n stringBuffer.push_back('\n');LINE
						      /*
Literal string parsing
						       */
<INITIAL,POST_UNION_STATE>\" BEGIN(STRING_STATE);stringBuffer.clear();COUNT
<STRING_STATE>\" BEGIN(mode); COPY_STRING_BUFFER;COUNT RETURN (STRING);
<STRING_STATE>\\["\\"''] stringBuffer.push_back('\a');COUNT
<STRING_STATE>\\a stringBuffer.push_back('\a');COUNT
<STRING_STATE>\\b stringBuffer.push_back('\a');COUNT
<STRING_STATE>\\f stringBuffer.push_back('\a');COUNT
<STRING_STATE>[^\"\n\\]+ stringBuffer.append(yytext);COUNT
						     /*
C-style comment parsing
						     */
<INITIAL,POST_UNION_STATE>\/\* BEGIN(C_COMMENT_STATE);
<C_COMMENT_STATE>[^*/\n]
<C_COMMENT_STATE>\*
<C_COMMENT_STATE>\/
<C_COMMENT_STATE>\*\/ BEGIN(mode);
<C_COMMENT_STATE>\n LINE

<REGEX_MODE>;\n BEGIN(LEX_MODES_STATE); COUNT LINE RETURN(';');
<REGEX_MODE>.+ yylval.string = new char[yyleng]; strcpy(yylval.string,yytext);COUNT LINE RETURN(REGULAR_EXPRESSION);
<REGEX_MODE>\n 


\/\/.*\n LINE // C++-style comment

\'[^\n\\]\' yylval.character = yytext[1];COUNT RETURN (CHARACTER); // Character literal


"%%" COUNT RETURN (PCNT_PCNT);
"%token" COUNT RETURN (PCNT_TOKEN);
"%type" COUNT RETURN (PCNT_TYPE);
"%union" COUNT RETURN (PCNT_UNION);
"%start" COUNT RETURN (PCNT_START);


[a-zA-Z_][a-zA-Z0-9_]* yylval.string = new char[yyleng]; strcpy(yylval.string,yytext);COUNT RETURN (ID);
<INITIAL,LEX_MODES_STATE>[{}] COUNT RETURN (yytext[0]);
\[] COUNT RETURN (SQUARE_BRACKETS);
[;:|<>*] COUNT RETURN (yytext[0]);

\n LINE
[\ \t] COUNT



%%

void changeMode()
{
    BEGIN(POST_UNION_STATE);
    mode = POST_UNION_STATE;
}
void lexRulesMode()
{
    BEGIN(LEX_MODES_STATE);
    mode = LEX_MODES_STATE;
}
void regexMode()
{
    BEGIN(REGEX_MODE);
}
