#include <iostream>
//int yyparse();
class Script_Type;
Script_Type* parse(const char* filename);
int main()
{
  parse("TestScript.txt");
  //yyparse();
  std::cout << "TestInput.ast program executable" << std::endl;
  return 0;
}
