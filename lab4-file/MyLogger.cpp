//
// Created by CONG YU on 2019/12/12.
//

#include <iostream>
#include "MyLogger.h"

MyLogger::MyLogger(ostream& output) {
    this->output_ = &output;
}

void MyLogger::SetLogVerbose(bool log_verbose) {
    this->log_verbose_ = log_verbose;
}

void MyLogger::SetLogDetail(bool log_detail) {
    this->log_detail_ = log_detail;
}

void MyLogger::SetLogFlookDetail(bool log_flook_detail) {
    this->log_flook_detail_ = log_flook_detail;
}


MyLogger::~MyLogger() {
    // possible to close file output stream
}

MyLogger::MyLogger() {
    this->output_ = &std::cout;
}

bool MyLogger::IsLogVerbose() const {
    return log_verbose_;
}

bool MyLogger::IsLogDetail() const {
    return log_detail_;
}

bool MyLogger::IsLogFlookDetail() const {
    return log_flook_detail_;
}


MyLogger &operator<<(MyLogger &logger, const std::string& log) {
    *( logger.output_ ) << log;
    return logger;
}

MyLogger &operator<<(MyLogger &logger,  int log) {
    *( logger.output_ ) << log;
    return logger;
}
