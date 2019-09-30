//
// Created by CONG YU on 2019/9/27.
//

#include "lexer.h"

using namespace std;
/**
 * convert a string to integer, if cannot, return -1
 * @param input - input string
 * @return integer repr by input, -1 if invalid
 */
int readInt(string input) {
    if (input.length() == 0) return -1;
    int val = 0;
    for (int i = 0; i < input.length(); ++i) {
        if (input[i] < '0' || input[i] > '9')
            return -1;
        val = val*10 + (input[i] - '0');
    }
    return val;
}

string readSymbol(string input) {
    return input;
}

char readMode(string input) {
    if (input.length() != 1) return 0;
    return input[0];
}

bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

bool isAlphabet(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

bool checkOperation(int operation) {
    // TODO
    return operation >= 1000 && operation <= 9999;
}

/**
 * symbol should satisfy:
 * (1) [a-Z][a-Z0-9]*
 * (2) Accepted symbols should be upto 16 characters long (not including terminations e.g. ‘\0’)
 * @param symbol
 * @return whether symbol is accepted
 */
bool checkSymbols(string symbol) {
    if (symbol.length() == 0) return false;
    if (symbol.length() > 16) return false;
    if (!isAlphabet(symbol[0])) return false;
    for (int i = 1; i < symbol.length(); ++i) {
        if (!isAlphabet(symbol[i]) && !isDigit(symbol[i])) return false;
    }
    return true;
}

bool checkMode(char mode) {
    return mode == 'R' || mode == 'E' || mode == 'I' || mode == 'A';
}

bool checkCount(int val) {
    // TODO
    return val >= 0;
}

bool checkOffset(int val) {
    return val >= 0;
}

