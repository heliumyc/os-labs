//
// Created by CONG YU on 2019/9/29.
//

#ifndef LINKER_TOKENIZER_H
#define LINKER_TOKENIZER_H

#include <fstream>
#include "errors.h"

class Tokenizer {
public:
    Tokenizer(std::istream* input, std::ostream* output);
    ~Tokenizer();

    int readInt(bool allowEOF);

    int readDefCount();

    int readUseCount();

    int readInstrCount(int baseAddr);

    int readAddr();

    std::string readSymbol();

    char readMode();

//    int readInstr();

    static bool checkDefCount(int count);

    static bool checkUseCount(int count);

    static bool checkAddr(int addr);

    static bool checkMode(char mode);

    static bool checkSymbol(std::string symbol);

    static bool checkInstrCount(int count, int baseAddr);

    static bool checkInstr(int instr);

    void printAll();

    bool reachEnd();

    int getLine();

    int getOffset();

    int getWordLen();

    int getLastOffset();

private:
    std::istream* input;
    std::ostream* output;
    std::string buffer;
    int line = 0;
    int offset = 0;
    int wordLen = 0;

    Error* error;

    bool newLine();

    std::string readWord();

    static bool isDelimiter(char c);

    static bool isAlphabet(char c);

    static bool isDigit(char c);

};

#endif //LINKER_TOKENIZER_H
