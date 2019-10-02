//
// Created by CONG YU on 2019/9/28.
//

#include "parser.h"

Module::Module(int addr) {
    this->size = 0;
    this->address = addr;
}

int Module::convertInstruction(char instructionType, int instruction,
        unordered_map<string, int> &symbolTable, unordered_map<string, int> &symbolRefCount, string& errorMsg) {

    // check if op is valid
    int convertedInstr = 0;
    int opcode = instruction/1000;
    int operand = instruction%1000;
    switch (instructionType) {
        case 'R': {
            if (!(instruction >= 0 && instruction <= 9999)) {
                convertedInstr = 9999;
                errorMsg = " Error: Illegal opcode; treated as 9999";
                break;
            }
            if (operand > this->size) {
                operand = 0;
                errorMsg = " Error: Relative address exceeds module size; zero used";
            }
            operand += this->address;
            // check
            convertedInstr = opcode*1000 + operand;

            break;
        }
        case 'E': {
            if (!(instruction >= 0 && instruction <= 9999)) {
                convertedInstr = 9999;
                errorMsg = " Error: Illegal opcode; treated as 9999";
                break;
            }
            // check if operand exceeds use list size
            if (operand < this->useList.size()) {
                string symbol = this->useList[operand];
                // check if symbol in the def list
                if (symbolTable.find(symbol) != symbolTable.end()) {
                    convertedInstr = opcode*1000 + symbolTable[symbol];
                    if (this->symbolRefCount.find(symbol) != this->symbolRefCount.end()) {
                        this->symbolRefCount[symbol] += 1;
                    }
                    else {
                        this->symbolRefCount.insert({symbol, 1});
                    }
                }
                else {
                    // use symbol not found in def list, 0 used
                    errorMsg = " Error: " + symbol + " is not defined; zero used";
                    convertedInstr = opcode*1000 + 0;
                }
            }
            else {
                // exceeds use list
                errorMsg = " Error: External address exceeds length of uselist; treated as immediate";
                convertedInstr = instruction;
            }


            break;
        }
        case 'I': {
            // check
            if (!(instruction >= 0 && instruction <= 9999)) {
                convertedInstr = 9999;
                errorMsg = " Error: Illegal immediate value; treated as 9999";
                break;
            }
            convertedInstr = instruction;

            break;
        }
        case 'A': {
            if (!(instruction >= 0 && instruction <= 9999)) {
                convertedInstr = 9999;
                errorMsg = " Error: Illegal opcode; treated as 9999";
                break;
            }
            // check if it is above machine size
            if (instruction > 512) {
                convertedInstr = (instruction/1000)*1000 + 0;
                errorMsg = " Error: Absolute address exceeds machine size; zero used";
            }
            else {
                convertedInstr = instruction;
            }
            break;
        }
        default: {break;}
    }
    return convertedInstr;
}
