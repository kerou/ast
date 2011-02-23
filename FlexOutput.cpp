#include "ParserOutputEgg.h"
using namespace std;
#include <stack>
#include <sstream>
std::unordered_map<std::string,unsigned int> LexCommand_Rule::rulesPerToken;

void outputFile(ofstream* outfile, const char* infileName)
{
    ifstream preamble(infileName);
    for ( ;; )
    {
        char string[100];
        preamble.getline(string,100);
        (*outfile) << string << endl;
        if (!preamble.good()) break;
    }
}

void ParserOutput::outputFlex()
{
    ofstream file("TestOutput.l");

    outputFile(&file,"FlexPreamble.txt");

    lexModes->outputFlexDeclarations(&file);
    LexCommand_Rule::clearMap();

    file << "%%" << endl;

    lexModes->outputFlexRules(&file);

    file << "%%" << endl;

    outputFile(&file,"FlexPostamble.txt");

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
    (*file) << "%x " << id << "_Mode" << endl;
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
        (*file) << "%x " << id << "_Mode" << number << endl;
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
        (*file) << '<' << mode << "_Mode" << '>' << regexes[0] << " copyString(); return " << id << ';' << endl << endl;
    }
    else
    {
        string s(id);
        unsigned int number = rulesPerToken[s]++;
        (*file) << '<' << mode << "_Mode" << '>' << regexes[0] << " pushMode(" << id << "_Mode" << number << "); appendToStringBuffer();" << endl;
        for (unsigned int i = 1; i < size-1; i++)
        {
            (*file) << '<' << id << "_Mode" << number << '>' << regexes[i] << " appendToStringBuffer();" << endl;
        }
        (*file) << '<' << id << "_Mode" << number  << '>' << regexes[size-1] << " popMode(); returnStringBuffer(); return " << id << ';' << endl << endl;
    }
}
































