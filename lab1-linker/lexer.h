//
// Created by CONG YU on 2019/9/27.
//

#ifndef LINKER_UTILS_H
#define LINKER_UTILS_H

#include <string>

bool isAlphabet(char c);

bool isDigit(char c);

int readInt(std::string input);

std::string readSymbol(std::string input);

char readMode(std::string input);

bool checkCount(int val);

bool checkOffset(int val);

bool checkSymbols(std::string symbol);

bool checkOperation(int operation);

bool checkMode(char mode);

#endif //LINKER_UTILS_H
