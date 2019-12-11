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
    explicit MyReader(istream* input);
    int GetInt();
    MyReader& operator >> (int& number);

private:
    istream* input;
    string current_line;
    stringstream current_line_stream;

    bool GetNextLine();
};

#endif //LAB4_FILE_MYREADER_H
