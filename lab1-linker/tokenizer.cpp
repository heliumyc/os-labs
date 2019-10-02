//
// Created by CONG YU on 2019/9/29.
//

#include "tokenizer.h"
#include <string>
#include <iostream>

using namespace std;

Tokenizer::Tokenizer(std::istream* input, std::ostream* output) {
    this->input = input;
    this->output = output;
    this->line = 0;
    this->offset = 1;
    this->error = new Error(output);
}

Tokenizer::~Tokenizer() {
    delete this->error;
}

bool Tokenizer::isDelimiter(char c) {
    return c == '\t' || c == '\n' || c ==' ';
}

bool Tokenizer::isAlphabet(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

bool Tokenizer::isDigit(char c) {
    return c >= '0' && c <= '9';
}

bool Tokenizer::reachEnd() {
    return this->input->eof();
}

bool Tokenizer::newLine() {
    if (getline(*this->input, this->buffer)) {
        this->line++;
        this->offset = 0;
        return true;
    }
    else return false;
}

std::string Tokenizer::readWord() {
    if (this->reachEnd()) return "";
    while (this->buffer.empty() || this->offset == this->buffer.length()) {
        bool hasNext = this->newLine();
        if (!hasNext) return "";
    }

    while (isDelimiter(this->buffer[this->offset])) {
        offset++;
        if (this->offset == this->buffer.length()) {
            bool hasNext = this->newLine();
            if (!hasNext) {
                return "";
            }
        }
    }
    this->wordLen = 0;
    for (int i = offset; (i < this->buffer.length()) && !isDelimiter(this->buffer[i]); ++i) {
        this->wordLen++;
    }
    this->offset += this->wordLen;
    return (this->buffer).substr(this->offset-this->wordLen, this->wordLen);
}

void Tokenizer::printAll() {
    string s;
    while (!(s=readWord()).empty()) {
        cout << line << ":" << offset + 1 - wordLen << " " << s << endl;
    }
    cout << line << ":" << offset+1;
}

int Tokenizer::readInt(bool allowEOF) {
    if (this->reachEnd()) {
        if (!allowEOF) {
            this->error->logSyntaxError(NUM_EXPECTED, this->getLine(), this->getOffset());
        }
        return -2;
    }
    std::string word = this->readWord();
    if (word.empty()) {
        if (!allowEOF) {
            this->error->logSyntaxError(NUM_EXPECTED, this->getLine(), this->getOffset());
        }
        return -2;
    }

    int val = 0;
    for (auto c: word) {
        if (c < '0' || c > '9') {
            this->error->logSyntaxError(NUM_EXPECTED, this->getLine(), this->getLastOffset());
            return -1;
        }
        val = val*10 + (c - '0');
    }
    return val;
}

int Tokenizer::readDefCount() {
    int cnt = this->readInt(true);
    if (cnt < 0) return cnt;
    if (!Tokenizer::checkDefCount(cnt)) {
        this->error->logSyntaxError(TOO_MANY_DEF_IN_MODULE, this->getLine(), this->getLastOffset());
        return -1;
    }
    return cnt;
}

int Tokenizer::readUseCount() {
    int cnt = this->readInt(false);
    if (cnt < 0) return -1;
    if (!Tokenizer::checkUseCount(cnt)) {
        this->error->logSyntaxError(TOO_MANY_USE_IN_MODULE, this->getLine(), this->getLastOffset());
        return -1;
    }
    return cnt;
}

int Tokenizer::readInstrCount(int baseAddr) {
    int cnt = this->readInt(false);
    if (cnt < 0) return -1;
    if (!Tokenizer::checkInstrCount(cnt, baseAddr)) {
        this->error->logSyntaxError(TOO_MANY_INSTR, this->getLine(), this->getLastOffset());
        return -1;
    }
    return cnt;
}

std::string Tokenizer::readSymbol() {
    if (this->reachEnd()) {
        this->error->logSyntaxError(SYM_EXPECTED, this->getLine(), this->getOffset());
        return "";
    }
    std::string word = this->readWord();
    if (word.empty()) {
        this->error->logSyntaxError(SYM_EXPECTED, this->getLine(), this->getOffset());
        return "";
    }
    if (!Tokenizer::checkSymbol(word)) {
        this->error->logSyntaxError(SYM_EXPECTED, this->getLine(), this->getLastOffset());
        return "";
    }
    return word;
}

char Tokenizer::readMode() {
    if (this->reachEnd()) {
        this->error->logSyntaxError(ADDR_EXPECTED, this->getLine(), this->getOffset());
        return 0;
    }
    std::string word = this->readWord();
    if (word.empty()) {
        this->error->logSyntaxError(ADDR_EXPECTED, this->getLine(), this->getOffset());
        return 0;
    }
    if (word.length() != 1 || !Tokenizer::checkMode(word[0])) {
        this->error->logSyntaxError(ADDR_EXPECTED, this->getLine(), this->getOffset());
        return 0;
    }
    return word[0];
}

bool Tokenizer::checkMode(char mode) {
    return mode == 'R' || mode == 'E' || mode == 'I' || mode == 'A';
}

bool Tokenizer::checkSymbol(std::string symbol) {
    if (symbol.length() == 0) return false;
    if (symbol.length() > 16) return false;
    if (!isAlphabet(symbol[0])) return false;
    for (unsigned long i = 1; i < symbol.length(); ++i) {
        if (!isAlphabet(symbol[i]) && !isDigit(symbol[i])) return false;
    }
    return true;
}

bool Tokenizer::checkUseCount(int count) {
    return count >= 0 && count <= 16;
}

bool Tokenizer::checkDefCount(int count) {
    return count >= 0 && count <= 16;
}

bool Tokenizer::checkInstrCount(int count, int baseAddr) {
    return (count+baseAddr) <= 512 && (count+baseAddr) >= 0;
}

int Tokenizer::getLine() {
    return this->line;
}

int Tokenizer::getOffset() {
    return this->offset+1;
}

int Tokenizer::getWordLen() {
    return this->wordLen;
}

int Tokenizer::getLastOffset() {
    return this->offset-this->wordLen+1;
}
