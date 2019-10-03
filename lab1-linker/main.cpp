#include <iostream>
#include <unordered_map>
#include <vector>
#include <iomanip>
#include "tokenizer.h"
#include "parser.h"

#define VERIFY_ELSE_BREAK(verification, fail) if(!(verification)) {failFlag = fail; break;}

using namespace std;

// for pass 1
vector<string> symbolList; // helper array to maintain the insertion order of hash table below
unordered_map<string, int> symbolTable; // using hash table rather than rbt. string for symbol, int for absolute addr
unordered_map<string, string> errorMsgOfSymbol;

// for pass 2
vector<Module> moduleList;

unordered_map<string, int> symbolUsageCount;

int pass_1(istream& input, ostream& output) {
    Tokenizer tokenizer(&input, &output);
    int baseAddr = 0;
    bool failFlag = false;
    vector<string> symbolsInCurModule;
    int moduleCnt = 0;
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

        symbolsInCurModule.clear();
        moduleCnt++;

        for (int i = 0; i < defCount; ++i) {
            string symbol = tokenizer.readSymbol();
            VERIFY_ELSE_BREAK(!symbol.empty(), true)

            int addr = tokenizer.readInt(false);
            VERIFY_ELSE_BREAK(addr >= 0, true)

            if (symbolTable.find(symbol) != symbolTable.end()) {
                // find multiple def! print ERROR!
                errorMsgOfSymbol.insert({symbol, "Error: This variable is multiple times defined; first value used"});
            }
            else {
                // insert symbol into table
                symbolList.push_back(symbol);
                symbolTable.insert({symbol, addr + baseAddr});
                symbolsInCurModule.push_back(symbol);
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

        // check if some var has too large addr, if so set it<  to 0
        for (const auto& sym: symbolsInCurModule) {
            if (symbolTable[sym] > baseAddr+instrCount-1) {
                output << "Warning: Module "<< moduleCnt << ": ";
                output << sym << " too big " << symbolTable[sym]-baseAddr  << " (max=" << max(0, instrCount-1);
                output << ") assume zero relative" << endl;
                symbolTable[sym] = baseAddr + 0;
            }
        }

        baseAddr += instrCount;

    }

    if (failFlag) {
        return -1;
    }

    return 1;
}

int pass_2(istream& input, ostream& output) {
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
            tokenizer.readInt(false);

            if (symbolTable[symbol] >= baseAddr) {
                module.defList.push_back(symbol);
            }
        }

        // use list
        int useCount = tokenizer.readUseCount();
        for (int i = 0; i < useCount; ++i) {
            string symbol = tokenizer.readSymbol();
            module.useList.push_back(symbol);
            if (symbolUsageCount.find(symbol) != symbolUsageCount.end()) {
                symbolUsageCount[symbol] += 1;
            }
            else {
                symbolUsageCount.insert({symbol, 1});
            }
        }

        // instruction list
        int instrCount = tokenizer.readInstrCount(baseAddr);
        module.size = instrCount;
        for (int i = 0; i < instrCount; ++i) {
            char mode = tokenizer.readMode();
            int instr = tokenizer.readInt(false);
            string errorMsg;
            int convertedInstr = module.convertInstruction(mode, instr, symbolTable, errorMsg);
            module.instructionList.push_back(convertedInstr);
            module.errorMsgList.push_back(errorMsg);
        }
        baseAddr += instrCount;
        moduleList.push_back(module);
    }

    return 1;
}

void printParsedResult(ostream& output) {
    if (moduleList.size() == 1 && moduleList[0].instructionList.size() == 0) return;

    output << "Symbol Table" << endl;
    for (auto & it : symbolList) {
        output << it << "=" << symbolTable[it];
        if (errorMsgOfSymbol.find(it) != errorMsgOfSymbol.end()) {
            output << " " << errorMsgOfSymbol[it];
        }
        output << endl;
    }

    output << endl;
    output << "Memory Map" << endl;
    int moduleCnt = 0;
    for(auto &module: moduleList) {
        moduleCnt++;
        for (unsigned long i = 0; i < module.instructionList.size(); ++i) {
            output << setfill('0') << setw(3) << (module.address + i) << ": ";
            output << setw(4) << module.instructionList[i];
            if (!module.errorMsgList[i].empty()) {
                output << module.errorMsgList[i];
            }
            output << endl;
        }
        for (const auto& symbol: module.useList) {
            if (module.symbolRefCount.find(symbol) == module.symbolRefCount.end()) {
                output << "Warning: Module " << moduleCnt << ": ";
                output << symbol <<" appeared in the uselist but was not actually used";
                output << endl;
            }
        }
    }

    bool firstFlag = true;
    // def but never used
    moduleCnt = 0;
    for (const auto& module: moduleList) {
        moduleCnt++;
        for (const auto& symbol: module.defList) {
            if (symbolUsageCount.find(symbol) == symbolUsageCount.end()) {
                if (firstFlag) {
                    output << endl;
                    firstFlag = false;
                }
                output << "Warning: Module " << moduleCnt << ": " << symbol << " was defined but never used" << endl;
            }
        }
    }
}

int main(int argc, char *argv[]) {
    ifstream myFile;
    string filePath = argv[1];
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

    printParsedResult(cout);


    return 0;
}