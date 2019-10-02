//
// Created by CONG YU on 2019/10/1.
//

#include "errors.h"


Error::Error(std::ostream* output) {
    this->output = output;
}

void Error::logSyntaxError(SYNTAX_ERROR type, int line, int offset) {
    *this->output << "Parse Error line " << line << " offset " << offset << ": ";
    *this->output << this->syntaxErrMsgMap[type] << std::endl;
}



