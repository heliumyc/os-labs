//
// Created by CONG YU on 2019/12/10.
//

#ifndef LAB4_FILE_IOSCHEDULER_H
#define LAB4_FILE_IOSCHEDULER_H

#include <queue>
#include <iostream>
#include "CommonTypes.h"

enum class IOSchedType {FIFO, SSTF, LOOK, CLOOK, FLOOK};

class IOScheduler {
protected:
    std::unique_ptr<Request> active_io{};
    int head = 0;
    int time = 0;

public:
    void IncrementTime();
    bool IsActive();
    bool IsCompleted();
    void ClearActive();
    virtual void MoveForward() = 0;
    virtual bool IsPending() = 0;
    virtual void AddNewIORequest(std::unique_ptr<Request>&& request) = 0;
    virtual void FetchNextAndStartNewIO() = 0;
    virtual ~IOScheduler()= default;;
};

class IOSchedulerFactory {
public:
    static std::unique_ptr<IOScheduler> CreateScheduler(IOSchedType io_sched_type);
};

#endif //LAB4_FILE_IOSCHEDULER_H
