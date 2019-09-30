#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include "tokenizer.h"

#define VERIFY_ELSE_BREAK(verification, error_statement) if(!verification) {error_statement;break;}

using namespace std;

vector<string> symbolList; // helper array to maintain the insertion order of hash table below
unordered_map<string, int> symbolTable; // using hash table rather than rbt. string for symbol, int for absolute addr

void pass_1(istream& input, ostream& output) {
    Tokenizer tokenizer(&input);
    for(int defCount = tokenizer.readInt(); defCount >= 0; defCount = tokenizer.readInt()) {
        VERIFY_ELSE_BREAK(Tokenizer::checkCount(defCount), {})
        for (int i = 0; i < defCount; ++i) {
            string symbol = tokenizer.readSymbol();
            VERIFY_ELSE_BREAK(Tokenizer::checkSymbol(symbol), {})
        }
    }
}

void pass_2(Tokenizer& tokenizer);

int main(int argc, char *argv[]) {
    ifstream myFile;
    myFile.open("input-1");

    pass_1(myFile, cout);

    myFile.close();


    return 0;
}