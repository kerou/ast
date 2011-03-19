#include "ParserOutputEgg.h"
#include "SymbolTable.h"
using namespace std;
#include <stack>
#include <sstream>
#include <iostream>
std::unordered_map<std::string,unsigned int> LexCommand_Rule::rulesPerToken;
extern std::unordered_map<std::string,void*> characterLiterals;

void outputFile(ofstream* outfile, const char* infileName)
{
    ifstream preamble(infileName);
    if (!preamble.is_open())
      {
	std::cout << "Failed to open file " << infileName << std::endl;
      }
    for ( ;; )
    {
        char string[100];
        preamble.getline(string,100);
        (*outfile) << string << endl;
        if (!preamble.good()) break;
    }
}
void outputCharacterLiteralFlexRule(ofstream* file)
{
    (*file) << '[';
    auto iter = characterLiterals.begin();
    while (iter != characterLiterals.end())
    {
        (*file) << iter->first[0];
        iter++;
    }
    (*file) << "] copyString(); count(yyleng); return yytext[0];" << endl;
}
void ParserOutput::outputFlex()
{
    ofstream file("TestOutput.l");

    {
      std::string fullFilename(resourceDirectory);
      fullFilename = fullFilename + "astres/FlexPreamble.txt";
      outputFile(&file,fullFilename.c_str());
    }
    
    lexModes->outputFlexDeclarations(&file);
    LexCommand_Rule::clearMap();

    file << "%%" << endl;

    lexModes->outputFlexRules(&file);

    outputCharacterLiteralFlexRule(&file);

    file << "[ \\t\\r] count(yyleng); whitespace();" << endl;
    file << "[\\n] endline(); whitespace();" << endl;

    file << "%%" << endl;
    {
      std::string fullFilename(resourceDirectory);
      fullFilename = fullFilename + "astres/FlexPostamble.txt";
      outputFile(&file,fullFilename.c_str());
    }

}
/// Flex declarations
void LexModes_Rule::outputFlexDeclarations(ofstream* file)
{
    for (unsigned int i = 0; i < modes.size(); i++)
    {
        modes[i]->outputFlexDeclarations(file);
    }
}

void LexMode_Rule::outputFlexDeclarations(ofstream* file)
{
    (*file) << "%s " << id << "_Mode" << endl;
    rules->outputFlexDeclarations(file);
}
void LexRules_Rule::outputFlexDeclarations(ofstream* file)
{
    for (unsigned int i = 0; i < rules.size(); i++)
    {
        rules[i]->outputFlexDeclaration(file);
    }
}
void LexRule_Rule::outputFlexDeclaration(ofstream* file)
{
    command->outputFlexDeclaration(file,id);
}
void LexCommand_Rule::outputFlexDeclaration(ofstream* file, char* id)
{
    unsigned int size = regexes.size();
    if (size != 1)
    {
        string s(id);
        unsigned int number = rulesPerToken[s]++;
        (*file) << "%s " << id << "_Mode" << number << endl;
    }
}

/// Flex rules
void LexModes_Rule::outputFlexRules(ofstream* file)
{
    for (unsigned int i = 0; i < modes.size(); i++)
    {
        modes[i]->outputFlexRules(file);
    }
}

void LexMode_Rule::outputFlexRules(ofstream* file)
{
    rules->outputFlexRules(file,id);
}
void LexRules_Rule::outputFlexRules(ofstream* file, char* mode)
{
    for (unsigned int i = 0; i < rules.size(); i++)
    {
        rules[i]->outputFlexRule(file,mode);
    }
}
void LexRule_Rule::outputFlexRule(ofstream* file, char* mode)
{
    command->outputFlexRule(file,id,mode);
}
void LexCommand_Rule::outputFlexRule(ofstream* file, char* id, char* mode)
{
    unsigned int size = regexes.size();
    if (size == 1)
    {
        (*file) << '<' << mode << "_Mode" << '>' << regexes[0] << " copyString(); count(yyleng); return " << id << ';' << endl << endl;
    }
    else
    {
        string s(id);
        unsigned int number = rulesPerToken[s]++;
        (*file) << '<' << mode << "_Mode" << '>' << regexes[0] << " pushMode(" << id << "_Mode" << number << "); count(yyleng); appendToStringBuffer();" << endl;
        for (unsigned int i = 1; i < size-1; i++)
        {
            (*file) << '<' << id << "_Mode" << number << '>' << regexes[i] << " count(yyleng); appendToStringBuffer();" << endl;
        }
        (*file) << '<' << id << "_Mode" << number  << '>' << regexes[size-1] << " popMode(); count(yyleng); returnStringBuffer(); return " << id << ';' << endl << endl;
    }
}

































