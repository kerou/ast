%{
	#include "TestOutput.h"
	int yylex();
	void yyerror(const char*);
	void pushMode(int mode);
	void popMode();
