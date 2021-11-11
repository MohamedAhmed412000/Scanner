#ifndef CODE_H
#define CODE_H

#include "iostream"
#include "fstream"
#include "string"
#include "vector"

using namespace std;

struct Token
{
    string Type;
    string Value;
};

void createFile(string);
void writeFile(string, string);
string readFile(string);
string getTokens(string);

#endif // CODE_H

