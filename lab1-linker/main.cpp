#include <iostream>
#include <unordered_map>
#include <vector>
#include "tokenizer.h"
#include "parser.h"

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
        output << it << "=" << symbolTable[it];
        output << endl;
    }
}

void pass_2(istream& input, ostream& output) {
    vector<Module> moduleList;
    Tokenizer tokenizer(&input);
    int baseAddr = 0;
    for(int defCount = tokenizer.readInt(); defCount >= 0; defCount = tokenizer.readInt()) {
        // create module
        Module module(baseAddr);
        // def list
        for (int i = 0; i < defCount; ++i) {
            string symbol = tokenizer.readSymbol();

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
        for (int i = 0; i < useCount; ++i) {
            string symbol = tokenizer.readSymbol();
            module.useList.push_back(symbol);
        }

        // instruction list
        int instrCount = tokenizer.readInt();
        for (int i = 0; i < instrCount; ++i) {
            char mode = tokenizer.readMode();
            int instr = tokenizer.readInt();
            int convertedInstr = module.convertInstruction(mode, instr, symbolTable);
            module.instructionList.push_back(convertedInstr);
        }
        baseAddr += instrCount;
        moduleList.push_back(module);
    }

    output << endl;
    output << "Memory Heap" << endl;
    for(auto &modIt: moduleList) {
        for (int i = 0; i < modIt.instructionList.size(); ++i) {
            output << setfill('0') << setw(3) << (modIt.address + i) << " " << modIt.instructionList[i] << endl;
        }
    }
}

int main(int argc, char *argv[]) {
    ifstream myFile;
    string filePath = "input-1";
    myFile.open(filePath);

    // pass 1
    pass_1(myFile, cout);

    myFile.close();

    // reopen file resource
    myFile.clear();
    myFile.open(filePath);

    // pass 2
    pass_2(myFile, cout);
    myFile.close();

    return 0;
}