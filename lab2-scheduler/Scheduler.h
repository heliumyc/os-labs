//
// Created by CONG YU on 2019/10/28.
//

#ifndef LAB2_SCHEDULER_SCHEDULER_H
#define LAB2_SCHEDULER_SCHEDULER_H

#include <queue>
#include "Process.h"

enum class SchedulerEnum {FCFS, LCFS, SRTF, RR, PRIO, PREPRIO};

class Scheduler {
    virtual void AddProcess(Process* p) = 0;
    virtual Process* GetNext() = 0;
    virtual void TryPreempt(Process* p, int curtime) = 0;

protected:
    std::queue<Process*> runQueue;
};

class SchedulerFactory {
public:
    static Scheduler* CreateScheduler(SchedulerEnum type);
};

#endif //LAB2_SCHEDULER_SCHEDULER_H
