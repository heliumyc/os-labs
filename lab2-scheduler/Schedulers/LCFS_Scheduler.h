//
// Created by CONG YU on 2019/10/28.
//

#ifndef LAB2_SCHEDULER_LCFS_SCHEDULER_H
#define LAB2_SCHEDULER_LCFS_SCHEDULER_H


#include "../Scheduler.h"

class LCFS_Scheduler: public Scheduler {
    void AddProcess(Process* p) override;
    Process* GetNextProcess() override;
    void TryPreempt(Process* p, int curtime) override;
};


#endif //LAB2_SCHEDULER_LCFS_SCHEDULER_H
