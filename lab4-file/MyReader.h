//
// Created by CONG YU on 2019/12/10.
//

#ifndef LAB4_FILE_MYREADER_H
#define LAB4_FILE_MYREADER_H

#include <istream>
#include <sstream>

using namespace std;
class MyReader {
public:
    explicit MyReader(std::unique_ptr<istream> input);
    unsigned GetUInt();
    int GetInt();
    friend MyReader& operator >> (MyReader& reader, int& number);
    explicit operator bool() const;

private:
    std::unique_ptr<istream> input;
    string current_line;
    stringstream current_line_stream;
    bool fail = false;

    bool GetNextLine();
};

#endif //LAB4_FILE_MYREADER_H
