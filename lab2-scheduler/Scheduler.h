//
// Created by CONG YU on 2019/10/28.
//

#ifndef LAB2_SCHEDULER_SCHEDULER_H
#define LAB2_SCHEDULER_SCHEDULER_H

#include <list>
#include <string>
#include "Process.h"

enum class SchedulerEnum {FCFS, LCFS, SRTF, RR, PRIO, PREPRIO};
std::string SchedulerEnumToString(SchedulerEnum s);

class Scheduler {
public:
    virtual void AddProcess(Process* p) = 0;
    virtual Process* GetNextProcess() = 0;
    virtual void TryPreempt(Process* p, int cur_time) = 0;

protected:
    std::list<Process*> run_queue;
    int quantum;
    int max_priority;

public:
    void setQuantum(int quantum);
    void setMaxPriority(int max_priority);
};

class SchedulerFactory {
public:
    static Scheduler* CreateScheduler(SchedulerEnum type, int quantum, int max_priority);
};

#endif //LAB2_SCHEDULER_SCHEDULER_H
