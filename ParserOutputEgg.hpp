#pragma once
#include <fstream>
#include <vector>
#include <unordered_map>
#include <string>
class BisonRules_Rule;
class BisonDeclarations_Rule;
class LexModes_Rule;
extern std::string resourceDirectory;
extern std::string outputfile;
extern class ParserOutput
{
public:
  ParserOutput();
  void addRules(BisonRules_Rule* _rules);
  void addDeclarations(BisonDeclarations_Rule* _declarations);
  void addLexModes(LexModes_Rule* _lexModes);
  void output();
private:
  void outputBison();
  void outputHeader();
  void outputCpp();
  void outputFlex();
  BisonRules_Rule* rules;
  BisonDeclarations_Rule* declarations;
  LexModes_Rule* lexModes;
} g_ParserOutput;
/// **** RULES SECTION ****
class Symbol_Rule
{
public:
  Symbol_Rule(){}
  virtual void outputRule(std::ofstream* file)=0;
  virtual void outputType(std::ofstream* file)=0;
};
class Symbols_Rule
{
public:
  Symbols_Rule(void);
  Symbols_Rule(Symbol_Rule* symbol);
  void addSymbol(Symbol_Rule* symbol);
  void outputRule(std::ofstream* file);
  void outputConstructorArguments(std::ofstream* file);
  void outputMemberDeclarations(std::ofstream* file);
  void outputConstructorCode(std::ofstream* file);
  void outputDestructorCode(std::ofstream* file);
  void outputGetDeclarations(std::ofstream* file);
  unsigned int size()
  {
    return symbols.size();
  }
private:
  std::vector<Symbol_Rule*> symbols;
};
class DerivationRules_Rule
{
public:
  DerivationRules_Rule(Symbols_Rule* symbols);
  void addRule(Symbols_Rule* symbols);
  void outputRule(std::ofstream* file, char* typeName);
  void outputType(std::ofstream* file, char* typeName);
  void outputTypeDefinition(std::ofstream* file, char* typeName);
  unsigned int size()
  {
    return rules.size();
  }
private:
  std::vector<Symbols_Rule*> rules;
};
class BisonRule_Rule
{
public:
  BisonRule_Rule(char* _name, DerivationRules_Rule* _rules);
  void outputRule(std::ofstream* file);
  void outputDeclaration(std::ofstream* file);
  void outputUnionMember(std::ofstream* file);
  void outputTypeDeclaration(std::ofstream* file);
  void outputTypeDefinition(std::ofstream* file);
  void outputType(std::ofstream* file);
private:
  char* name;
  DerivationRules_Rule* rules;
};
class BisonRules_Rule
{
public:
  BisonRules_Rule(BisonRule_Rule* rule);
  void addRule(BisonRule_Rule* rule);
  void outputUnionMembers(std::ofstream* file);
  void outputDeclarations(std::ofstream* file);
  void outputRules(std::ofstream* file);
  void outputTypeDeclarations(std::ofstream* file);
  void outputTypeDefinitions(std::ofstream* file);
  void outputTypes(std::ofstream* file);
private:
  std::vector<BisonRule_Rule*> rules;
};


/// Derived possibilities
class Symbol_Rule_ID: public Symbol_Rule
{
public:
  Symbol_Rule_ID(char* _name);
  void outputRule(std::ofstream* file);
  void outputType(std::ofstream* file);
private:
  char* name;
};
class Symbol_Rule_CHARACTER: public Symbol_Rule
{
public:
  Symbol_Rule_CHARACTER(char _character);
  void outputRule(std::ofstream* file);
  void outputType(std::ofstream* file);
private:
  char character;
  char nullTerminator;
};
class Symbol_Rule_STRING: public Symbol_Rule
{
public:
  Symbol_Rule_STRING(char* _string);
  void outputRule(std::ofstream* file);
  void outputType(std::ofstream* file);
private:
  char* string;
};
class Symbol_Rule_List: public Symbol_Rule
{
public:
  Symbol_Rule_List(char* _name, bool _canBeEmpty);
  void outputRule(std::ofstream* file);
  void outputType(std::ofstream* file);
  void outputListRule(std::ofstream* file);
private:
  char* name;
  bool canBeEmpty;
};

/// **** DECLARATIONS SECTION ****
class BisonDeclaration_Rule;
class BisonDeclarations_Rule
{
public:
  BisonDeclarations_Rule(BisonDeclaration_Rule* declaration);
  void addRule(BisonDeclaration_Rule* declaration);
  void outputDeclarations(std::ofstream* file);
  void outputUnionMembers(std::ofstream* file);
  void outputAliasDeclarations(std::ofstream* file);
private:
  std::vector<BisonDeclaration_Rule*> declarations;

};
class BisonDeclaration_Rule
{
public:
  virtual void outputDeclaration(std::ofstream* file) {}
  virtual void outputUnionMember(std::ofstream* file) {}
  virtual void outputAliasDeclaration(std::ofstream* file) {}
};
/// Bison declaration derivations
class SymTableDeclaration_Rule;
class StartDeclaration_Rule;
class TokenDeclaration_Rule;
class TypeDeclaration_Rule;
class SymTableInput_Rule;
class SymTableOutput_Rule;
class SymTableNotEqual_Rule;
class BisonDeclaration_Rule1: public BisonDeclaration_Rule
{
public:
  BisonDeclaration_Rule1(SymTableDeclaration_Rule* _symTable)
  {
    symTable = _symTable;
  }
private:
  SymTableDeclaration_Rule* symTable;
};
class BisonDeclaration_Rule2: public BisonDeclaration_Rule
{
public:
  BisonDeclaration_Rule2(StartDeclaration_Rule* rule) {}
};
class BisonDeclaration_Rule3: public BisonDeclaration_Rule
{
public:
  BisonDeclaration_Rule3(TokenDeclaration_Rule* _rule);
private:
  void outputDeclaration(std::ofstream* file);
  void outputUnionMember(std::ofstream* file);
  void outputAliasDeclaration(std::ofstream* file);
  TokenDeclaration_Rule* rule;
};
class BisonDeclaration_Rule4: public BisonDeclaration_Rule
{
public:
  BisonDeclaration_Rule4(TypeDeclaration_Rule* rule) {}
};
class BisonDeclaration_Rule5: public BisonDeclaration_Rule
{
public:
  BisonDeclaration_Rule5(SymTableInput_Rule* _tableInput)
  {
    tableInput = _tableInput;
  }
private:
  SymTableInput_Rule* tableInput;
};
class BisonDeclaration_Rule6: public BisonDeclaration_Rule
{
public:
  BisonDeclaration_Rule6(SymTableOutput_Rule* _tableOutput)
  {
    tableOutput = _tableOutput;
  }
private:
  SymTableOutput_Rule* tableOutput;
};
class BisonDeclaration_Rule7: public BisonDeclaration_Rule
{
public:
  BisonDeclaration_Rule7(SymTableNotEqual_Rule* _tableNotEqual)
  {
    tableNotEqual = _tableNotEqual;
  }
private:
  SymTableNotEqual_Rule* tableNotEqual;
};
class SymTableDeclaration_Rule
{
public:
  SymTableDeclaration_Rule(std::vector<char*>* _typeList, char* _id)
  {
    typeList = _typeList;
    id = _id;
  }
private:
  std::vector<char*>* typeList;
  char* id;
};
class SymTableInput_Rule
{
public:
  SymTableInput_Rule(char* _tableName, char* _inputTokenName)
  {
    tableName = _tableName;
    inputTokenName = _inputTokenName;
  }
private:
  char* tableName;
  char* inputTokenName;
};
class SymTableOutput_Rule
{
public:
  SymTableOutput_Rule(char* _tableName, char* _outputTokenName)
  {
    tableName = _tableName;
    outputTokenName = _outputTokenName;
  }
private:
  char* tableName;
  char* outputTokenName;
};
class SymTableNotEqual_Rule
{
public:
  SymTableNotEqual_Rule(char* _tableName, char* _notEqualTokenName)
  {
    tableName = _tableName;
    notEqualTokenName = _notEqualTokenName;
  }
private:
  char* tableName;
  char* notEqualTokenName;
};
class TokenList_Rule;
class TokenDeclaration_Rule
{
public:
  TokenDeclaration_Rule(TokenList_Rule* _list);
  void outputDeclaration(std::ofstream* file);
  void outputUnionMember(std::ofstream* file);
  void outputAliasDeclaration(std::ofstream* file);
private:
  TokenList_Rule* list;
};
class Token_Rule
{
public:
  Token_Rule(char* _id);
  virtual void outputDeclaration(std::ofstream* file)=0;
  virtual void outputUnionMember(std::ofstream* file)=0;
  virtual void outputAliasDeclaration(std::ofstream* file)=0;
  void outputSemanticAction(std::ofstream* file);
  void outputConstructorArguments(std::ofstream* file);
  const char* getId()
  {
    return id;
  }
  enum Type
  {
    token1,
    token2,
  };
  virtual Type getType()=0;
protected:
  char* id;
};
class TokenList_Rule
{
public:
  TokenList_Rule(Token_Rule* token);
  void addToken(Token_Rule* token);
  void outputDeclaration(std::ofstream* file);
  void outputUnionMembers(std::ofstream* file);
  void outputAliasDeclarations(std::ofstream* file);
private:
  std::vector<Token_Rule*> rules;
};
class Token_Rule1: public Token_Rule
{
public:
  Token_Rule1(char* _id);
  void outputDeclaration(std::ofstream* file);
  void outputUnionMember(std::ofstream* file);
  void outputAliasDeclaration(std::ofstream* file);
  Type getType()
  {
    return token1;
  }
private:
};
class Token_Rule2: public Token_Rule
{
public:
  Token_Rule2(char* _id, char* _string);
  void outputDeclaration(std::ofstream* file);
  void outputUnionMember(std::ofstream* file);
  void outputAliasDeclaration(std::ofstream* file);
  Type getType()
  {
    return token2;
  }
  const char* getString()
  {
    return string;
  }
private:
  char* string;
};
/// Lex modes section
class LexCommand_Rule
{
public:
  void add(char* regex);
  void outputFlexRule(std::ofstream* file, char* id, char* mode);
  void outputFlexDeclaration(std::ofstream* file, char* id);
  static void clearMap()
  {
    rulesPerToken.clear();
  }
private:
  static std::unordered_map<std::string,unsigned int> rulesPerToken;
  std::vector<char*> regexes;
};
class LexRule_Rule
{
public:
  LexRule_Rule(char* _id, LexCommand_Rule* _command);
  void outputFlexRule(std::ofstream* file, char* mode);
  void outputFlexDeclaration(std::ofstream* file);
private:
  char* id;
  LexCommand_Rule* command;
};
class LexRules_Rule
{
public:
  void add(LexRule_Rule* rule);
  void outputFlexRules(std::ofstream* file, char* mode);
  void outputFlexDeclarations(std::ofstream* file);
private:
  std::vector<LexRule_Rule*> rules;
};
class LexMode_Rule
{
public:
  LexMode_Rule(char* _id, LexRules_Rule* _rules);
  void outputFlexRules(std::ofstream* file);
  void outputFlexDeclarations(std::ofstream* file);
  void outputBisonCodeInsertion(std::ofstream* file, unsigned int which);
  void outputModeChangingAliasDeclaration(std::ofstream* file);
  void outputModeChangingAliasRule(std::ofstream* file);
  char* getId()
  {
    return id;
  }
private:
  char* id;
  LexRules_Rule* rules;
};
class LexModes_Rule
{
public:
  void add(LexMode_Rule* mode);
  void outputFlexRules(std::ofstream* file);
  void outputFlexDeclarations(std::ofstream* file);
  void outputBisonCodeInsertions(std::ofstream* file);
private:
  std::vector<LexMode_Rule*> modes;
};
















































