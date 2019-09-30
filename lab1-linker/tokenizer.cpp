//
// Created by CONG YU on 2019/9/29.
//

#include "tokenizer.h"
#include <string>
#include <iostream>

#include <utility>
using namespace std;

Tokenizer::Tokenizer(std::istream* input, std::ostream* error) {
    this->input = input;
    this->error = error;
    this->line = 1;
    this->offset = 1;
}

Tokenizer::~Tokenizer() {
//    (*this->input).ignore(INT_MAX);
//    this->input.reset();
//    this->error.reset();
}

int Tokenizer::readInt() {
    return 0;
}

std::string Tokenizer::readSymbol() {

    return std::string();
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
        *error << s << endl;
    }
}



bool isDelimiter(char c) {
    return c == '\t' || c == '\n' || c ==' ';
}
