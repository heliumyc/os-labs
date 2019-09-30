//
// Created by CONG YU on 2019/9/29.
//

#ifndef LINKER_TOKENIZER_H
#define LINKER_TOKENIZER_H

#include <fstream>

bool isDelimiter(char c);

class Tokenizer {
public:
    Tokenizer(std::istream* input, std::ostream* error);
    ~Tokenizer();

    int readInt();

    std::string readSymbol();

    char readMode();

    bool checkCount(int count);

    bool checkSymbol(std::string symbol);

    bool checkInstr(int instr);

    void printAll();

private:
    std::istream* input;
    std::ostream* error;
    int line;
    int offset;
    int word_length = 0;
    std::string buffer;

    std::string readWord();
    bool newLine();
};

#endif //LINKER_TOKENIZER_H
