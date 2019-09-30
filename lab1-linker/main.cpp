#include <iostream>
#include <fstream>
#include <unordered_map>
#include "lexer.h"
#include "parser.h"
#include "tokenizer.h"

#define VERIFY_ELSE_BREAK(verification, error_statement) if(!verification) {error_statement;break;}

using namespace std;

vector<string> symbolList; // helper array to maintain the insertion order of hash table below
unordered_map<string, int> symbolTable; // using hash table rather than rbt. string for symbol, int for absolute addr

void pass_1(istream& input, ostream& output) {
    Tokenizer tokenizer(&input);
    tokenizer.printAll();
}

void pass_2(Tokenizer& tokenizer);

int main(int argc, char *argv[]) {
    ifstream myFile;
    myFile.open("input-1");

    pass_1(myFile, cout);

    myFile.close();


    return 0;
}