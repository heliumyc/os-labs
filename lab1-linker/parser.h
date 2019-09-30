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
    vector<string> useList;
    vector<int> instructions;

    // constructor
    Module(int addr);

    // TODO add error parsing phrase
    int convertInstruction(char instructionType, int instruction, unordered_map<string, int> &symbolTable);

};

#endif //LINKER_PARSER_H
