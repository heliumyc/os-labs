//
// Created by CONG YU on 2019/12/10.
//

#ifndef LAB4_FILE_IOSCHEDULER_H
#define LAB4_FILE_IOSCHEDULER_H

#include <queue>
#include <iostream>
#include "CommonTypes.h"
#include "MyLogger.h"

enum class IOSchedType {FIFO, SSTF, LOOK, CLOOK, FLOOK};

class IOScheduler {
protected:
    std::unique_ptr<Request> active_io = nullptr;
    int head = 0;
    int time = 0;
    int direction = 1;

    // final summary
    std::vector<std::unique_ptr<Request>> finished_requests {};
    int total_movement = 0;
    double total_turnaround = 0;
    double total_wait_time = 0;
    int max_wait_time = 0;
    int request_num = 0;
    MyLogger logger;

public:
    void Start();
    void SetTime(int& current_time);
    bool IsActive();
    bool IsCompleted();
    void ClearActive();
    void SetLogger(MyLogger& my_logger);
    void LogSummary();
    void MoveForward();
    void LogNew(const std::unique_ptr<Request>& new_request);
    void LogNext();
    virtual bool IsPending() = 0;
    virtual void AddNewIORequest(std::unique_ptr<Request>& request) = 0;
    virtual void FetchNext() = 0;
    virtual ~IOScheduler();
};

class IOSchedulerFactory {
public:
    static std::unique_ptr<IOScheduler> CreateScheduler(IOSchedType io_sched_type);
};

#endif //LAB4_FILE_IOSCHEDULER_H
