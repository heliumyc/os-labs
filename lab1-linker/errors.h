//
// Created by CONG YU on 2019/10/1.
//

#ifndef LINKER_ERRORS_H
#define LINKER_ERRORS_H

#import <ostream>
#include <unordered_map>

enum SYNTAX_ERROR {
    NUMBER_EXPECTED,
    SYMBOL_EXPECTED,
    ADDR_EXPECTED,
    SYM_TOO_LONG,
    TOO_MANY_DEF_IN_MODULE,
    TOO_MANY_USE_IN_MODULE,
    TOO_MANY_INSTR
};

std::unordered_map<SYNTAX_ERROR, std::string> syntaxErrMsgMap = {
        {NUMBER_EXPECTED, "NUMBER_EXPECTED"},
        {SYMBOL_EXPECTED, "SYMBOL_EXPECTED"},
        {ADDR_EXPECTED, "ADDR_EXPECTED"},
        {SYM_TOO_LONG, "SYM_TOO_LONG"},
        {TOO_MANY_DEF_IN_MODULE, "TOO_MANY_DEF_IN_MODULE"},
        {TOO_MANY_USE_IN_MODULE, "TOO_MANY_USE_IN_MODULE"},
        {TOO_MANY_INSTR, "TOO_MANY_INSTR"},
};

class Error {
public:
    std::ostream* output{};
    explicit Error(std::ostream* output);

    void logSyntaxError(SYNTAX_ERROR type, int line, int offset);

};

#endif //LINKER_ERRORS_H
