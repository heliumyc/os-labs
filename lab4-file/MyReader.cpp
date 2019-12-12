//
// Created by CONG YU on 2019/12/10.
//

#include "MyReader.h"

MyReader::MyReader(std::unique_ptr<istream> input) {
    this->input = std::move(input);
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

unsigned MyReader::GetUInt() {
    unsigned next;
    if (current_line_stream >> next) {
        return next;
    } else {
        if (GetNextLine()) {
            return GetUInt();
        } else {
            fail = true;
            return next;
        }
    }
}

int MyReader::GetInt() {
    int next;
    if (current_line_stream >> next) {
        return next;
    } else {
        if (GetNextLine()) {
            return GetInt();
        } else {
            fail = true;
            return next;
        }
    }
}

MyReader& operator>>(MyReader& reader, int &number) {
    unsigned tmp = reader.GetUInt();
    if (tmp == -1) {
        reader.fail = true;
    } else {
        number = tmp;
    }
    return reader;
}

MyReader::operator bool() const {
    return !fail;
}
