//
// Created by CONG YU on 2019/12/12.
//

#ifndef LAB4_FILE_MYLOGGER_H
#define LAB4_FILE_MYLOGGER_H

#include <ostream>

using namespace std;
class MyLogger {

private:
    bool log_verbose_ = false;
    bool log_detail_ = false;
    bool log_flook_detail_ = false;
    ostream* output_ = nullptr;
public:
    MyLogger();
    explicit MyLogger(ostream& output);
    void SetLogVerbose(bool log_verbose);
    void SetLogDetail(bool log_detail);
    void SetLogFlookDetail(bool log_flook_detail);
    bool IsLogVerbose() const;
    bool IsLogDetail() const;
    bool IsLogFlookDetail() const;

    friend MyLogger& operator << (MyLogger& logger, const std::string& log);
    friend MyLogger& operator << (MyLogger& logger, const int& log);
    // if using other ostream, need to close it;
    ~MyLogger();
};


#endif //LAB4_FILE_MYLOGGER_H
