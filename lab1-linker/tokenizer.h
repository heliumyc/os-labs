//
// Created by CONG YU on 2019/9/29.
//

#ifndef LINKER_TOKENIZER_H
#define LINKER_TOKENIZER_H

#include <fstream>

class Tokenizer {
public:
    Tokenizer(std::istream* input);
    ~Tokenizer();

    int readInt();

    std::string readSymbol();

    char readMode();

//    int readInstr();

    static bool checkCount(int count);

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
    std::string buffer;
    int line = 0;
    int offset = 0;
    int wordLen = 0;

    bool newLine();

    std::string readWord();

    static bool isDelimiter(char c);

    static bool isAlphabet(char c);

    static bool isDigit(char c);

};

#endif //LINKER_TOKENIZER_H
