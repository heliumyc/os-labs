#include <iostream>
#include <fstream>
#include <unordered_map>
#include "lexer.h"
#include "parser.h"
#include "tokenizer.h"

using namespace std;

vector<string> symbolList; // helper array to maintain the insertion order of hash table below
unordered_map<string, int> symbolTable; // using hash table rather than rbt. string for symbol, int for absolute addr

void pass1();

void pass2();

int main(int argc, char *argv[]) {
    ifstream myFile;
    myFile.open("input-1");

//    ofstream error;
//    ostream error(cout);


    Tokenizer tokenizer(&myFile, &cout);
    tokenizer.printAll();

    myFile.close();

    return 0;
}