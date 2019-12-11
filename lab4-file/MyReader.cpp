//
// Created by CONG YU on 2019/12/10.
//

#include "MyReader.h"

MyReader::MyReader(istream* input) {
    this->input = input;
}

bool MyReader::GetNextLine() {
    if (getline(*input, current_line)) {
        if (current_line[0] == '#') {
            return this->GetNextLine();
        } else {
            current_line_stream = stringstream(current_line);
            return true;
        }
    } else {
        return false;
    }
}

int MyReader::GetInt() {
    int next_int;
    if (current_line_stream >> next_int) {
        return next_int;
    } else {
        if (GetNextLine()) {
            return GetInt();
        } else {
            return -1;
        }
    }
}

MyReader& MyReader::operator>>(int &number) {
    int tmp = this->GetInt();
    if (tmp == -1) {
        fail = true;
    } else {
        number = tmp;
    }
    return *this;
}

MyReader::operator bool() const {
    return !fail;
}
