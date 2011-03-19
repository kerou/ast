#define ALIASING
#include "ParserOutputEgg.hpp"
#include "SymbolTable.hpp"
std::unordered_map<std::string,void*> characterLiterals;
std::unordered_map<std::string,Token_Rule*> terminalTokens;
std::unordered_map<std::string,Symbol_Rule_List*> listRules;
std::unordered_map<std::string,LexMode_Rule*> lexModeTable;
std::unordered_map<std::string,Token_Rule2*> stringAliases;
#include <cstring>
#include <fstream>
using namespace std;
const char preamble[] =
"\tint yylex();\n\
\tvoid yyerror(const char*);\n";
void outputListUnionMembers(ofstream* file)
{
  auto iter = listRules.begin();
  while (iter != listRules.end())
    {
      (*file) << '\t' << iter->first << "_List* " << iter->first << "_List_Return;" << endl;
      iter++;
    }
}
void outputListDeclarations(ofstream* file)
{
  auto iter = listRules.begin();
  while (iter != listRules.end())
    {
      (*file) << "%type <" << iter->first << "_List_Return> " << iter->first << "_List" << endl;
      (*file) << "%type <" << iter->first << "_List_Return> " << iter->first << "_ListIter" << endl;
      iter++;
    }
}
void outputListRules(ofstream* file)
{
  auto iter = listRules.begin();
  while (iter != listRules.end())
    {
      iter->second->outputListRule(file);
      iter++;
    }
}
void outputCharacterLiteralAliasDeclarations(ofstream* file)
{
  auto iter = characterLiterals.begin();
  while (iter != characterLiterals.end())
    {
      char literal = iter->first[0];
      int literalInt = literal;
      (*file) << "%type <Character_Alias> LitChar" << literalInt << " \""  << literal << "\"\n";
      iter++;
    }
}
void outputModeChangingAliasDeclarations(ofstream* file)
{
  auto iter = lexModeTable.begin();
  while (iter != lexModeTable.end())
    {
      iter->second->outputModeChangingAliasDeclaration(file);
      iter++;
    }
}
void LexMode_Rule::outputModeChangingAliasDeclaration(ofstream* file)
{
  (*file) << "%type <" << id << "_Return> " << id << "_Alias" << endl;
}
void outputModeChangingAliasRules(ofstream* file)
{
  auto iter = lexModeTable.begin();
  while (iter != lexModeTable.end())
    {
      iter->second->outputModeChangingAliasRule(file);
      iter++;
    }
}
void LexMode_Rule::outputModeChangingAliasRule(ofstream* file)
{
  (*file) << id << "_Alias:" << endl;
  (*file) << "\t{pushMode(" << id << "_Mode);} " << id << " {popMode(); $$ = $2;}" << endl;
  (*file) << ';' << endl;
}
void outputCharacterLiteralDeclarations(ofstream* file)
{
  (*file) << "%type <characterWhitespace> ";
  auto iter = characterLiterals.begin();
  while (iter != characterLiterals.end())
    {
      (*file) << '\'' << iter->first[0] << "' ";
      iter++;
    }
  (*file) << endl;
}
void outputCharacterLiteralAliasRules(ofstream* file)
{
  auto iter = characterLiterals.begin();
  while (iter != characterLiterals.end())
    {
      char literal = iter->first[0];
      int literalInt = literal;
      (*file) << "LitChar" << literalInt << ":\n\t '" << literal << "' {$$ = new CharacterLiteral_Type('" << literal << "',$1);}\n;\n";
      iter++;
    }
}
void outputAliasRules(ofstream* file)
{
  auto iter = terminalTokens.begin();
  while (iter != terminalTokens.end())
    {
      (*file) << iter->first << "_Alias:\n\t" << iter->first;
      (*file) << " {$$ = new " << iter->first << "_Type($1.string,$1.preceedingWhitespace);}" << endl;
      if (iter->second->getType() == Token_Rule::token2)
        {
          Token_Rule2* rule2 = (Token_Rule2*)iter->second;
          (*file) << "|\terror {$$ = NULL; yyerror(\"Expected " << rule2->getString() << " token\"); yyerrok;}" << endl;
        }
      else
        {
          (*file) << "|\terror {$$ = NULL; yyerror(\"Expected " << iter->first << " token\"); yyerrok;}" << endl;
        }
      (*file) << ';' << endl;
      iter++;
    }
}
void ParserOutput::outputBison()
{
  ofstream file(outputfile + ".y");
  /// Output %{..%}
  file << "%{" << endl;
  file << "#include \"" << outputfile << ".h\"" << endl;
  file << preamble;
  lexModes->outputBisonCodeInsertions(&file);
  file << '\t' << lexModeTable.begin()->first << "_Type* parserReturn;" << endl;
  file << "%}" << endl;
  /// Output union declaration
  file << "%union\n{\n\t/// Union members for all of the user-defined rules\n";
  rules->outputUnionMembers(&file);
  file << "\t/// Union members for list rules\n";
  outputListUnionMembers(&file);
  file << "\t/// Union members for the terminal token aliases\n";
  declarations->outputUnionMembers(&file);
  file << "\t/// Rule used for all character literal aliases\n";
  file << "\tCharacterLiteral_Type* Character_Alias;" << endl;
  file << "\tstruct" << endl;
  file << "\t{" << endl;
  file << "\t\tchar* string;" << endl;
  file << "\t\tchar* preceedingWhitespace;" << endl;
  file << "\t}matchedText;" << endl;
  file << "\tchar* characterWhitespace;" << endl;
  file << '}' << endl;

  file << "/// Setting the return types for the user-defined rules\n";
  rules->outputDeclarations(&file);
  file << "/// List rules\n";
  outputListDeclarations(&file);

  file << "/// User-defined terminal tokens\n";
  declarations->outputDeclarations(&file);
  file << "/// All character literals used" << endl;
  outputCharacterLiteralDeclarations(&file);
  file << "/// Mode changing alias\n";
  file << "%type <" << lexModeTable.begin()->first << "_Return> " << "StartRule_Automatic" << endl;
  outputModeChangingAliasDeclarations(&file);
  file << "/// Terminal token aliases\n";
  declarations->outputAliasDeclarations(&file);
  file << "/// Character literal aliases\n";
  outputCharacterLiteralAliasDeclarations(&file);
  file << "%%" << endl;
  file << "StartRule_Automatic:" << endl;
  file << '\t' << lexModeTable.begin()->first << "_Alias {$$ = $1; parserReturn = $1;}" << endl;
  file << ';' << endl;
  file << "/// User-defined rules\n";
  rules->outputRules(&file);
  file << "/// List rules\n";
  outputListRules(&file);
  file << "/// Mode changing alias rules" << endl;
  outputModeChangingAliasRules(&file);
  file << "/// Terminal token alias rules\n";
  outputAliasRules(&file);
  file << "/// Character literal alias rules\n";
  outputCharacterLiteralAliasRules(&file);
  file << "%%" << endl;
}
/// Flex rules telling bison about the lex modes
void LexModes_Rule::outputBisonCodeInsertions(ofstream* file)
{
  (*file) << "\tvoid pushMode(int mode);" << endl;
  (*file) << "\tvoid popMode();" << endl;
  for (unsigned int i = 0; i < modes.size(); i++)
    {
      modes[i]->outputBisonCodeInsertion(file,i+1);
    }
}
void LexMode_Rule::outputBisonCodeInsertion(ofstream* file, unsigned int which)
{
  (*file) << "\t#define " << id << "_Mode " << which << endl;
}
void BisonDeclarations_Rule::outputDeclarations(ofstream* file)
{
  for (unsigned int i = 0; i < declarations.size(); i++)
    {
      declarations[i]->outputDeclaration(file);
    }
}
void BisonDeclarations_Rule::outputUnionMembers(ofstream* file)
{
  for (unsigned int i = 0; i < declarations.size(); i++)
    {
      declarations[i]->outputUnionMember(file);
    }
}
void BisonDeclarations_Rule::outputAliasDeclarations(ofstream* file)
{
  for (unsigned int i = 0; i < declarations.size(); i++)
    {
      declarations[i]->outputAliasDeclaration(file);
    }
}
BisonDeclaration_Rule3::BisonDeclaration_Rule3(TokenDeclaration_Rule* _rule)
{
  rule = _rule;
}
void BisonDeclaration_Rule3::outputDeclaration(ofstream* file)
{
  rule->outputDeclaration(file);
}
void BisonDeclaration_Rule3::outputUnionMember(ofstream* file)
{
  rule->outputUnionMember(file);
}
void BisonDeclaration_Rule3::outputAliasDeclaration(ofstream* file)
{
  rule->outputAliasDeclaration(file);
}
void TokenDeclaration_Rule::outputDeclaration(ofstream* file)
{
  (*file) << "%token <matchedText>" << endl;
  list->outputDeclaration(file);
}
void TokenDeclaration_Rule::outputUnionMember(ofstream* file)
{
  list->outputUnionMembers(file);
}
void TokenDeclaration_Rule::outputAliasDeclaration(ofstream* file)
{
  list->outputAliasDeclarations(file);
}
void TokenList_Rule::outputDeclaration(ofstream* file)
{
  for (unsigned int i = 0; i < rules.size(); i++)
    {
      rules[i]->outputDeclaration(file);
    }
}
void TokenList_Rule::outputUnionMembers(ofstream* file)
{
  for (unsigned int i = 0; i < rules.size(); i++)
    {
      rules[i]->outputUnionMember(file);
    }
}
void TokenList_Rule::outputAliasDeclarations(ofstream* file)
{
  for (unsigned int i = 0; i < rules.size(); i++)
    {
      rules[i]->outputAliasDeclaration(file);
    }
}
Token_Rule::Token_Rule(char* _id)
{
  id = _id;
  terminalTokens[id] = this;
}
void Token_Rule1::outputDeclaration(ofstream* file)
{
  (*file) << '\t' << id << '\n';
}
void Token_Rule1::outputUnionMember(ofstream* file)
{
  (*file) << '\t' << id << "_Type* " << id << "_Type_Return;\n";
}
void Token_Rule1::outputAliasDeclaration(ofstream* file)
{
  (*file) << "%type <" << id << "_Type_Return> " << id << "_Alias\n";
}
void Token_Rule2::outputDeclaration(ofstream* file)
{
  (*file) << '\t' << id << '\n';//'" \"" << string << "\"\n";
}
void Token_Rule2::outputUnionMember(ofstream* file)
{
  (*file) << '\t' << id << "_Type* " << id << "_Type_Return;\n";
}
void Token_Rule2::outputAliasDeclaration(ofstream* file)
{
  (*file) << "%type <" << id  << "_Type_Return> " << id << "_Alias \"" << string << "\"\n";
}
void BisonRules_Rule::outputUnionMembers(ofstream* file)
{
  for (unsigned int i = 0; i < rules.size(); i++)
    {
      rules[i]->outputUnionMember(file);
    }
}
void BisonRules_Rule::outputDeclarations(ofstream* file)
{
  for (unsigned int i = 0; i < rules.size(); i++)
    {
      rules[i]->outputDeclaration(file);
    }
}
void BisonRules_Rule::outputRules(ofstream* file)
{
  for (unsigned int i = 0; i < rules.size(); i++)
    {
      rules[i]->outputRule(file);
      (*file) << endl << ';' << endl;
    }
}
void BisonRule_Rule::outputRule(ofstream* file)
{
  unsigned int length = strlen(name);
  char typeName[length + strlen("_Type")];
  strcpy(typeName,name);
  strcpy(typeName+length,"_Type");
  (*file) << name << ':' << endl;
  rules->outputRule(file,typeName);
}
void DerivationRules_Rule::outputRule(ofstream* file, char* typeName)
{
  for (unsigned int i = 0; i < rules.size(); i++)
    {
      if (i == 0)
        (*file) << '\t';
      else
        (*file) << endl << "|\t";
      rules[i]->outputRule(file);
      (*file) << "{$$ = new " << typeName;
      if (rules.size() != 1) (*file) << i+1;
      (*file) << "(";
      for (unsigned int ii = 0; ii < rules[i]->size(); ii++)
        {
          (*file) << '$' << ii+1;
          if (ii < rules[i]->size()-1)
            (*file) << ',';
        }
      (*file) << ");}";
    }
}
void Symbols_Rule::outputRule(ofstream* file)
{
  if (symbols.size() == 0)
    {
      (*file) << "/* Empty rule */";
    }
  else
    for (unsigned int i = 0; i < symbols.size(); i++)
      {
        symbols[i]->outputRule(file);
      }
}
void Symbol_Rule_ID::outputRule(ofstream* file)
{
  if (terminalTokens.find(name) == terminalTokens.end() && lexModeTable.find(name) == lexModeTable.end())
    (*file) << name << ' ';
  else
    (*file) << name << "_Alias ";
}
void Symbol_Rule_CHARACTER::outputRule(ofstream* file)
{
#ifndef ALIASING
  (*file) << '"' << character << '"' << ' ';
#else
  int literalInt = character;
  (*file) << "LitChar" << literalInt << ' ';
#endif
}
void Symbol_Rule_STRING::outputRule(ofstream* file)
{
  Token_Rule2* token = stringAliases[string];
  (*file) << token->getId() << "_Alias ";
}
void Symbol_Rule_List::outputRule(ofstream* file)
{
  (*file) << name << "_List ";
}
void Symbol_Rule_List::outputListRule(ofstream* file)
{
  (*file) << name << "_List:" << endl;
  (*file) << "\t/* Empty line */ {$$ = new " << name << "_List();}" << endl;
  (*file) << "|\t" << name << "_ListIter "  << " {$$ = $1;}" << endl;
  (*file) << ';' << endl;
  if (terminalTokens.find(name) == terminalTokens.end())
    {
      if (lexModeTable.find(name) == lexModeTable.end())
        {
          (*file) << name << "_ListIter:" << endl;
          (*file) << '\t' << name  << " {$$ = new " << name << "_List(); $$->add($1);}" << endl;
          (*file) << "|\t" << name << "_ListIter " << name  << " {$$ = $1; $$->add($2);}" << endl;
          (*file) << ';' << endl;
        }
      else
        {
          (*file) << name << "_ListIter:" << endl;
          (*file) << '\t' << "{pushMode(" << name << "_Mode);} " << name << " {popMode(); $$ = new " << name << "_List(); $$->add($2);}" << endl;
          (*file) << "|\t" << name << "_ListIter " << "{pushMode(" << name << "_Mode);} " << name  << " {popMode(); $$ = $1; $$->add($3);}" << endl;
          (*file) << ';' << endl;
        }
    }
  else
    {
      (*file) << name << "_ListIter:" << endl;
      (*file) << '\t' << name << "_Alias" << " {$$ = new " << name << "_List(); $$->add($1);}" << endl;
      (*file) << "|\t" << name << "_ListIter " << name << "_Alias" << " {$$ = $1; $$->add($2);}" << endl;
      (*file) << ';' << endl;
    }
}
BisonDeclarations_Rule::BisonDeclarations_Rule(BisonDeclaration_Rule* declaration)
{
  if (declaration != NULL)
    {
      declarations.push_back(declaration);
    }
}
void BisonDeclarations_Rule::addRule(BisonDeclaration_Rule* declaration)
{
  declarations.push_back(declaration);
}
/// Declarations output
void BisonRule_Rule::outputDeclaration(ofstream* file)
{
  (*file) << "%type <" << name << "_Return> " << name << endl;
}
void BisonRule_Rule::outputUnionMember(ofstream* file)
{
  (*file) << '\t' << name << "_Type* " << name << "_Return;" << endl;
}
TokenDeclaration_Rule::TokenDeclaration_Rule(TokenList_Rule* _list)
{
  list = _list;
}
TokenList_Rule::TokenList_Rule(Token_Rule* token)
{
  rules.push_back(token);
}
void TokenList_Rule::addToken(Token_Rule* token)
{
  rules.push_back(token);
}
Token_Rule1::Token_Rule1(char* _id)
  :Token_Rule(_id)
{
}
Token_Rule2::Token_Rule2(char* _id, char* _string)
  :Token_Rule(_id)
{
  string = _string;
  stringAliases[string] = this;
}
Symbol_Rule_CHARACTER::Symbol_Rule_CHARACTER(char _character)
{
  nullTerminator = '\0';
  character = _character;
  if (characterLiterals.find(&character) == characterLiterals.end())
    {
      characterLiterals[string(&character)] = NULL;
    }
}
Symbol_Rule_ID::Symbol_Rule_ID(char* _name)
{
  name = _name;
}
Symbol_Rule_List::Symbol_Rule_List(char* _name, bool _canBeEmpty)
{
  name = _name;
  canBeEmpty = _canBeEmpty;
  listRules[name] = this;
}

























