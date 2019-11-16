//
// Created by CONG YU on 2019/11/12.
//

#include "MyReader.h"

MyReader::MyReader(istream* input) {
    this->input = input;
}

int MyReader::GetNextLine() {
    if (getline(*input, current_line)) {
        if (current_line[0] == '#') {
            return this->GetNextLine();
        } else {
            current_line_stream = stringstream(current_line);
            return 1;
        }
    } else {
        return -1;
    }
}

int MyReader::GetInt() {
    int next_int;
    if (current_line_stream >> next_int) {
        return next_int;
    } else {
        if (GetNextLine() > 0) {
            return GetInt();
        } else {
            return -1;
        }
    }
}

char MyReader::GetChar() {
    string next_char;
    if (current_line_stream >> next_char) {
        return next_char[0];
    } else {
        if (GetNextLine() > 0) {
            return GetChar();
        } else {
            return -1;
        }
    }
}

int MyReader::GetNextInstruction(Instruction& instruction) {
    char operation;
    int operand;
    if ((operation = GetChar()) >= 0) {
        operand = GetInt();
        instruction.operand = operand;
        instruction.operation = operation;
        return 1;
    } else {
        return 0;
    }
}

int MyReader::GetNextVma(VirtualMemoryArea &vma) {
    vma.start_vpage = GetInt();
    vma.end_vpage = GetInt();
    vma.write_protected = GetInt();
    vma.file_mapped = GetInt();
    return 1;
}
