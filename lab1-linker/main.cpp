#include <iostream>
#include <unordered_map>
#include <vector>
#include "tokenizer.h"
#include "parser.h"
#include "errors.h"

#define VERIFY_ELSE_BREAK(verification, fail) if(!(verification)) {failFlag = fail; break;}

using namespace std;

vector<string> symbolList; // helper array to maintain the insertion order of hash table below
unordered_map<string, int> symbolTable; // using hash table rather than rbt. string for symbol, int for absolute addr

int pass_1(istream& input, ostream& output) {
    Tokenizer tokenizer(&input, &output);
    int baseAddr = 0;
    bool failFlag = false;
    while(!tokenizer.reachEnd()) {
        int defCount = tokenizer.readDefCount();
        // def list
        if (defCount == -1) {
            failFlag = true;
            break;
        } else if (defCount == -2) {
            failFlag = false;
            break;
        }
//        VERIFY_ELSE_BREAK(defCount >= 0, false)

        for (int i = 0; i < defCount; ++i) {
            string symbol = tokenizer.readSymbol();
            VERIFY_ELSE_BREAK(!symbol.empty(), true)

            int addr = tokenizer.readInt(false);
            VERIFY_ELSE_BREAK(addr >= 0, true)

            if (symbolTable.find(symbol) != symbolTable.end()) {
                // find multiple def! print ERROR!
            }
            else {
                // insert symbol into table
                symbolList.push_back(symbol);
                symbolTable.insert({symbol, addr + baseAddr});
            }
        }
        if (failFlag) break;

        // use list
        int useCount = tokenizer.readUseCount();
        VERIFY_ELSE_BREAK(useCount >= 0, true)
        for (int i = 0; i < useCount; ++i) {
            string symbol = tokenizer.readSymbol();
            VERIFY_ELSE_BREAK(!symbol.empty(), true)
        }
        if (failFlag) break;

        // instruction list
        int instrCount = tokenizer.readInstrCount(baseAddr);
        VERIFY_ELSE_BREAK(instrCount >= 0, true)
        for (int i = 0; i < instrCount; ++i) {
            char mode = tokenizer.readMode();
            VERIFY_ELSE_BREAK(mode != 0, true)

            int instr = tokenizer.readInt(false);
            VERIFY_ELSE_BREAK(instr >= 0, true)
        }
        if (failFlag) break;
        baseAddr += instrCount;
    }

    if (failFlag) {
        return -1;
    }

    output << "Symbol Table" << endl;
    for (auto & it : symbolList) {
        output << it << "=" << symbolTable[it];
        output << endl;
    }

    return 1;
}

int pass_2(istream& input, ostream& output) {
    vector<Module> moduleList;
    Tokenizer tokenizer(&input, &output);
    int baseAddr = 0;
    while(!tokenizer.reachEnd()) {
        // create module
        Module module(baseAddr);
        // def list
        int defCount = tokenizer.readDefCount();
        if (defCount < 0) break;
        for (int i = 0; i < defCount; ++i) {
            string symbol = tokenizer.readSymbol();
            int addr = tokenizer.readInt(false);

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
        int useCount = tokenizer.readUseCount();
        for (int i = 0; i < useCount; ++i) {
            string symbol = tokenizer.readSymbol();
            module.useList.push_back(symbol);
        }

        // instruction list
        int instrCount = tokenizer.readInstrCount(baseAddr);
        for (int i = 0; i < instrCount; ++i) {
            char mode = tokenizer.readMode();
            int instr = tokenizer.readInt(false);
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

    return 1;
}

int main(int argc, char *argv[]) {
    ifstream myFile;
    string filePath = "test";
    myFile.open(filePath);

    // pass 1
    int flag = pass_1(myFile, cout);
    myFile.close();

    if (flag < 0) {
        return 0;
    }

    cout << flush;
    // reopen file resource
    myFile.clear();
    myFile.open(filePath);

    // pass 2
    pass_2(myFile, cout);
    myFile.close();

    return 0;
}