//
// Created by CONG YU on 2019/9/29.
//

#include "tokenizer.h"
#include <string>
#include <iostream>

#include <utility>
using namespace std;

Tokenizer::Tokenizer(std::istream* input) {
    this->input = input;
    this->line = 0;
    this->offset = 1;
}

Tokenizer::~Tokenizer() {
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
    if (this->buffer.empty() || this->offset == this->buffer.length()) {
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
    this->word_length = 0;
    for (int i = offset; (i < this->buffer.length()) && !isDelimiter(this->buffer[i]); ++i) {
        this->word_length++;
    }
    this->offset += this->word_length;
    return (this->buffer).substr(this->offset-this->word_length, this->word_length);
}

void Tokenizer::printAll() {
    string s;
    while (!(s=readWord()).empty()) {
        cout << line << ":" << offset+1-word_length << " " << s << endl;
    }
    cout << line << offset+1;
}

int Tokenizer::readInt() {
    if (this->reachEnd()) return -1;
    std::string word = this->readWord();
    if (word.empty()) return -1;
    int val = 0;
    for (auto c: word) {
        if (c < '0' || c > '9')
            return -1;
        val = val*10 + (c - '0');
    }
    return val;
}

std::string Tokenizer::readSymbol() {
    if (this->reachEnd()) return "";
    std::string word = this->readWord();
    if (word.empty()) return "";
    return word;
}

bool Tokenizer::checkSymbol(std::string symbol) {
    if (symbol.length() == 0) return false;
    if (symbol.length() > 16) return false;
    if (!isAlphabet(symbol[0])) return false;
    for (int i = 1; i < symbol.length(); ++i) {
        if (!isAlphabet(symbol[i]) && !isDigit(symbol[i])) return false;
    }
    return true;
}

bool Tokenizer::isDelimiter(char c) {
    return c == '\t' || c == '\n' || c ==' ';
}

bool Tokenizer::isAlphabet(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

bool Tokenizer::isDigit(char c) {
    return c >= '0' && c <= '9';;
}


