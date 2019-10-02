//
// Created by CONG YU on 2019/9/28.
//

#ifndef LINKER_PARSER_H
#define LINKER_PARSER_H

#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

class Module {
public:
    int address;
    int size;
    vector<string> defList;
    vector<string> useList;
    vector<int> instructionList;
    vector<string> errorMsgList;
    unordered_map<string, int> symbolRefCount;

    // constructor
    explicit Module(int addr);

    // TODO add error parsing phrase
    int convertInstruction(char instructionType, int instruction, unordered_map<string, int> &symbolTable, string& errorMsg);

};

#endif //LINKER_PARSER_H
