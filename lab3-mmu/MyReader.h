//
// Created by CONG YU on 2019/11/12.
//

#ifndef LAB3_MMU_MYREADER_H
#define LAB3_MMU_MYREADER_H

#include <istream>
#include <sstream>
#include "DataTypes.h"
#include "Process.h"

using namespace std;
class MyReader {
public:
    explicit MyReader(istream* input);
    int GetInt();
    char GetChar();
    int GetNextInstruction(Instruction &instruction);
    int GetNextVma(VirtualMemoryArea& vma);

private:
    istream* input;
    string current_line;
    stringstream current_line_stream;

    int GetNextLine();
};


#endif //LAB3_MMU_MYREADER_H
