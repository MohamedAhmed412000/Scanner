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

string readFile(string filePath){
	fstream newfile; 
	newfile.open(filePath, ios::in);	// Open a file to perform read operation using file object 
	string tp, input = ""; 
	if (newfile.is_open()) {	// Checking whether the file is open 
		while(getline(newfile, tp)) {	// Read data from file object and put it into string. 
			input += tp; 
			input += " "; 
		}
		newfile.close(); //close the file object. 
	}

	int index;
	for (int i = input.length()-1; i > 0; i--) {
		if (!(input[i] == ' ' || input[i] == '\t' || input[i] == '\n')) {
			index = i;
			break;
		}
	}
	return input.substr(0, index+1);
}
void clearFile(string filename) { 
	ofstream ofs; 
	ofs.open(filename, ofstream::out | ofstream::trunc); 
	ofs.close(); 
} 
void writeFile(string input,string filename) { 
	ofstream log(filename, ios_base::app | ios_base::out); 
	log << input<<"\n"; 
}

bool isChar(char in){
	return ((in > 96 && in < 123) || (in > 64 && in < 91));
}
bool isNum(char in){
	return (in > 47 && in < 58);
}
bool isOper(char in) {
	if (in == '+' || in == '-' || in == '*' || in == '/' || in == '=' || in == '(' || in == ')' || in == ';')
		return true;
	return false;
}
bool isReservedWord(string in){
	string Word[8] = {"if", "then", "else", "end", "repeat", "until", "read", "write"};
	for (int i = 0; i < 8; i++){
		if (in == Word[i])
			return true;
	}
	return false;
}
bool isBreakChar(char in){
	if (in == '[' || in == ' ' || in == '\t' || in == '\n' || in == '{' || in == ']' || in == ':')
		return true;
	return false;
}

int index = 0;	// Global Index to use it with every call

Token getToken(string Text){
	int state = 0, s;
	Token x;
	while (state != 13) {
		switch (state) {
		case 0:	// START STATE
			s = index;
			if (isChar(Text[index]))
				state = 1;	// CHAR STATE
			else if (isNum(Text[index]))
				state = 3;	// NUM STATE
			else if (Text[index] == '{')
				state = 4;	// COMMENT STATE
			else if (isOper(Text[index]))
				state = 5;	// REGULAR OPERATOR STATE
			else if (Text[index] == '<' || Text[index] == '>')
				state = 6;	// GREATER OR LESS STATE
			else if (Text[index] == ':')
				state = 7;	// ASSIGN STATE
			break;
		/*=========================================================================*/
		case 1:	// CHAR STATE
			if (isNum(Text[index]))
				state = 2;	// IDENTIFIER NUM STATE
			if (isBreakChar(Text[index]) || (index == (Text.length()-1)) || isOper(Text[index])) {
				int end = isBreakChar(Text[index]) ? index-s : index-s+1;
				end = isOper(Text[index]) ? index-s : end;
			 	x.Value = Text.substr(s, end);
				if (isReservedWord(x.Value))
					x.Type = "Reserved word";
				else
					x.Type = "Identifier";
				state = 13;	// ACCEPTING STATE
			}
			break;
		/*=========================================================================*/
		case 2:	// IDENTIFIER NUM STATE
			if (isChar(Text[index]))
				state = 1;	// CHAR STATE 
			if (isBreakChar(Text[index]) || (index == (Text.length()-1)) || isOper(Text[index])) {
				x.Type = "Identifier";
				int end = isBreakChar(Text[index]) ? index-s : index-s+1;
				end = isOper(Text[index]) ? index-s : end;
				x.Value = Text.substr(s, end);
				state = 13;	// ACCEPTING STATE
			}
			break;
		/*=========================================================================*/
		case 3:	// CONSTANT NUM STATE
			if (isChar(Text[index])) {
				state = 12;	// ERROR STATE
				x.Value = "Wrong Constant (Added characters)";
			}
			if (isBreakChar(Text[index]) || (index == (Text.length()-1)) || isOper(Text[index])) {
				x.Type = "Number";
				int end = isBreakChar(Text[index]) ? index-s : index-s+1;
				end = isOper(Text[index]) ? index-s : end;
				x.Value = Text.substr(s, end);
				state = 13;	// ACCEPTING STATE
			}
			break;
		/*=========================================================================*/
		case 4:	// COMMENT STATE
			if (Text[index] == '}')
				state = 0;	// START STATE
			if ((index == (Text.length()-1)) && Text[index] != '}') {
				state = 12;	// ERROR STATE
				x.Value = "Incomplete Comment";
			}
			break;
		/*=========================================================================*/
		case 5:	// OPERATOR STATE
			x.Type = "Special Symbol";
			x.Value = Text[index-1];
			state = 13;	// ACCEPTING STATE
			break;
		/*=========================================================================*/
		case 6:	// GREATER OR LESS STATE
			if (Text[index] == '=')
				state = 8;	// COMPLETE EQUAL STATE
			else{
				x.Type = "Special Symbol";
				x.Value = Text[index-1];
				state = 13;	// ACCEPTING STATE
			}
			break;
		/*=========================================================================*/
		case 7:	// ASSIGN STATE
			if (Text[index] == '=')
				state = 8;	// COMPLETE EQUAL STATE 
			else {
				state = 12;	// ERROR
				x.Value = "Do you mean := (Assign Statement)";
			}
			break;
		/*=========================================================================*/
		case 8:	// COMPLETE EQUAL STATE
			state = 13;	// ACCEPTING STATE
			x.Type = "Special Symbol";
			x.Value = Text.substr(s, 2);	
			break;
		/*=========================================================================*/
		case 12:	// ERROR STATE
			state = 13;	// ACCEPTING STATE
			x.Type = "ERROR";
			while (!(isBreakChar(Text[index]) || (index == (Text.length()-1))))
				index++;
			break;
		/*=========================================================================*/
		default:
			break;
		}
		if (state != 13 && (index != (Text.length()-1)))
			index++;
	}
	return x;
}

int main() {
	vector<Token> tokenList;
	string input = readFile("file.txt");
	string fileName = "tokens.txt";
	clearFile(fileName);
	while (index != (input.length()-1)) {
		tokenList.push_back(getToken(input));
		writeFile(tokenList.back().Value + ", " + tokenList.back().Type, fileName);
		cout << tokenList.back().Value << ", " << tokenList.back().Type << endl;
	}

	system("pause");
	return 0;
}