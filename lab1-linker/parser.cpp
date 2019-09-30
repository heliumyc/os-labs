//
// Created by CONG YU on 2019/9/28.
//

#include "parser.h"

Module::Module(int addr) {
    this->address = addr;
}

int Module::convertInstruction(char instructionType, int instruction, unordered_map<string, int> &symbolTable) {
    int convertedInstr = 0;
    switch (instructionType) {
        case 'R': {
            int opcode = instruction/1000;
            int operand = instruction%1000;
            operand += this->address;
            // check
            convertedInstr = opcode*1000 + operand;

            break;
        }
        case 'E': {
            int opcode = instruction/1000;
            int operand = instruction%1000;

            // check if operand exceeds use list size
            if (operand < this->useList.size()) {
                string symbol = this->useList[operand];
                // check if symbol in the def list
                if (symbolTable.find(symbol) != symbolTable.end()) {
                    convertedInstr = opcode*1000 + symbolTable[symbol];
                }
                else {
                    // use symbol not found in def list
                }
            }
            else {

            }


            break;
        }
        case 'I': {
            // check
            convertedInstr = instruction;

            break;
        }
        case 'A': {
            // check if it is above machine size
            convertedInstr = instruction;
            break;
        }
        default: {break;}
    }
    return convertedInstr;
}
