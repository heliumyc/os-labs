#include <iostream>
#include <unordered_map>
#include <vector>
#include "tokenizer.h"

#define VERIFY_ELSE_BREAK(verification, error_statement) if(!(verification)) {error_statement;break;}

using namespace std;

vector<string> symbolList; // helper array to maintain the insertion order of hash table below
unordered_map<string, int> symbolTable; // using hash table rather than rbt. string for symbol, int for absolute addr

void pass_1(istream& input, ostream& output) {
    Tokenizer tokenizer(&input);
    int baseAddr = 0;
    for(int defCount = tokenizer.readInt(); defCount >= 0; defCount = tokenizer.readInt()) {

        // def list
        VERIFY_ELSE_BREAK(Tokenizer::checkCount(defCount), {})
        for (int i = 0; i < defCount; ++i) {
            string symbol = tokenizer.readSymbol();
            VERIFY_ELSE_BREAK(Tokenizer::checkSymbol(symbol), {})

            int addr = tokenizer.readInt();

            if (symbolTable.find(symbol) != symbolTable.end()) {
                // find multiple def! print ERROR!
            }
            else {
                // insert symbol into table
                symbolList.push_back(symbol);
                symbolTable.insert({symbol, addr + baseAddr});
            }
        }

        // use list
        int useCount = tokenizer.readInt();
        VERIFY_ELSE_BREAK(Tokenizer::checkCount(useCount), {})
        for (int i = 0; i < useCount; ++i) {
            string symbol = tokenizer.readSymbol();
            VERIFY_ELSE_BREAK(Tokenizer::checkSymbol(symbol), {})
        }

        // instruction list
        int instrCount = tokenizer.readInt();
        VERIFY_ELSE_BREAK(Tokenizer::checkInstrCount(instrCount, baseAddr), {})
        for (int i = 0; i < instrCount; ++i) {
            char mode = tokenizer.readMode();
            VERIFY_ELSE_BREAK(Tokenizer::checkMode(mode), {})

            int instr = tokenizer.readInt();
            VERIFY_ELSE_BREAK(Tokenizer::checkInstr(instr), {})
        }
        baseAddr += instrCount;
    }
    output << "Symbol Table" << endl;
    for (auto & it : symbolList) {
        cout << it << "=" << symbolTable[it];
        cout << endl;
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