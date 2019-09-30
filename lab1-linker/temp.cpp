#include <iostream>
#include <fstream>
#include <unordered_map>
#include "lexer.h"
#include "parser.h"

// parse error, print error message
#define TRY_READ_ELSE_EXIT(error_statement) if (!(file >> input)){error_statement;exit(1);}
#define VERIFY_ELSE_EXIT(verification, error_statement) if(!verification) {error_statement;exit(1);}

using namespace std;

vector<string> symbolList; // helper array to maintain the insertion order of hash table below
unordered_map<string, int> symbolTable; // using hash table rather than rbt. string for symbol, int for absolute addr

void pass_1(ifstream& file) {
    string input;
    int offset = 0;
    while (file >> input) {

        // define list
//        TRY_READ_ELSE_EXIT()
        int defCount = readInt(input);
        VERIFY_ELSE_EXIT(checkCount(defCount), )
        for (int i = 0; i < defCount; ++i) {
            TRY_READ_ELSE_EXIT()
            string symbol = readSymbol(input);
            VERIFY_ELSE_EXIT(checkSymbols(symbol),)

            TRY_READ_ELSE_EXIT()
            int val = readInt(input);
            VERIFY_ELSE_EXIT(checkOffset(val),)

            // check duplicated symbol declaration
            if (symbolTable.find(symbol) != symbolTable.end()) {
                // find multiple def! print ERROR!
            }
            else {
                // insert symbol into table
                symbolList.push_back(symbol);
                symbolTable.insert({symbol, val+offset});
            }
        }

        // use list
        TRY_READ_ELSE_EXIT()
        int useCount = readInt(input);
        VERIFY_ELSE_EXIT(checkCount(useCount),)
        for (int i = 0; i < useCount; ++i) {
            TRY_READ_ELSE_EXIT()
            string symbol = readSymbol(input);
            VERIFY_ELSE_EXIT(checkSymbols(symbol),)
        }

        // instruction text
        TRY_READ_ELSE_EXIT()
        int instrCount = readInt(input);
        VERIFY_ELSE_EXIT(checkCount(instrCount),)
        for (int i = 0; i < instrCount; ++i) {
            TRY_READ_ELSE_EXIT()
            char mode = readMode(input);
            VERIFY_ELSE_EXIT(checkMode(mode),)

            TRY_READ_ELSE_EXIT()
            int instruction = readInt(input);
            VERIFY_ELSE_EXIT(checkOperation(instruction),)
        }
        offset += instrCount;
    }

    //
    // TODO
    // print symbol table for test

    cout << "Symbol Table" << endl;
    for (auto & it : symbolList) {
        cout << it << "=" << symbolTable[it];
        cout << endl;
    }
}

void pass_2(ifstream& file) {
    // since syntax errors have been scanned in pass 1
    // so in pass 2, just parse it directly
    vector<Module> moduleList;
    string input;
    int offset = 0;

    while (file >> input) {
        // create module
        Module module(offset);

        // define list
        // useful information has already been parsed in pass 1, so just leave it
        int defCount = readInt(input);
        for (int i = 0; i < defCount; ++i) {
            TRY_READ_ELSE_EXIT()
            string symbol = readSymbol(input);
            TRY_READ_ELSE_EXIT()
            int val = readInt(input);
        }

        // use list
        TRY_READ_ELSE_EXIT()
        int useCount = readInt(input);
        for (int i = 0; i < useCount; ++i) {
            TRY_READ_ELSE_EXIT()
            string symbol = readSymbol(input);
            module.useList.push_back(symbol);
        }

        // instruction text
        TRY_READ_ELSE_EXIT()
        int instrCount = readInt(input);
        for (int i = 0; i < instrCount; ++i) {
            TRY_READ_ELSE_EXIT()
            char mode = readMode(input);

            TRY_READ_ELSE_EXIT()
            int instruction = readInt(input);
            int convertedInstr = module.convertInstruction(mode, instruction, symbolTable);
            module.instructions.push_back(convertedInstr);
        }
        module.size = instrCount;
        offset += instrCount;
        moduleList.push_back(module);
    }


    // print memory map
    cout << "Memory Map" << endl;
    for(auto &modIt: moduleList) {
        for (int i = 0; i < modIt.instructions.size(); ++i) {
            cout << setfill('0') << setw(3) << (modIt.address + i) << " " << modIt.instructions[i] << endl;
        }
    }
}


int main(int argc, char *argv[]) {
    ifstream myFile;

    // file path
    string filePath = argv[1];

    // pass 1
    myFile.open(filePath);
    pass_1(myFile);
    myFile.close();

    myFile.clear();
    cout << endl;

    // pass 2
    myFile.open(filePath);
    pass_2(myFile);
    myFile.close();

    return 0;
}