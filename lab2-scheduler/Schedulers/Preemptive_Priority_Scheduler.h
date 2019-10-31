//
// Created by CONG YU on 2019/10/28.
//

#ifndef LAB2_SCHEDULER_PREEMPTIVE_PRIORITY_SCHEDULER_H
#define LAB2_SCHEDULER_PREEMPTIVE_PRIORITY_SCHEDULER_H


#include "../Scheduler.h"

class Preemptive_Priority_Scheduler: public Scheduler {
    void AddProcess(Process* p) override;
    Process* GetNextProcess() override;
    void TryPreempt(Process* p, int curtime) override;
};


#endif //LAB2_SCHEDULER_PREEMPTIVE_PRIORITY_SCHEDULER_H
