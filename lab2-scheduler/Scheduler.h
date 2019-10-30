//
// Created by CONG YU on 2019/10/28.
//

#ifndef LAB2_SCHEDULER_SCHEDULER_H
#define LAB2_SCHEDULER_SCHEDULER_H

#include "../Process.h"

enum scheduler_t {FCFS, LCLS, SRTF, RR, PRIO, PREPRIO};

class Scheduler {
    virtual void addProcess(Process* p) = 0;
    virtual Process* getNext() = 0;
    virtual void testPreempt(Process* p, int curtime) = 0;
};

class SchedulerFactory {
public:
    Scheduler* createScheduler(scheduler_t type);
};

#endif //LAB2_SCHEDULER_SCHEDULER_H
