//
// Created by CONG YU on 2019/10/28.
//

#ifndef LAB2_SCHEDULER_SRTF_SCHEDULER_H
#define LAB2_SCHEDULER_SRTF_SCHEDULER_H


#include "../Scheduler.h"

class SRTF_SCheduler: public Scheduler {
    void AddProcess(Process* p) override;
    Process* GetNext() override;
    void TryPreempt(Process* p, int curtime) override;
};


#endif //LAB2_SCHEDULER_SRTF_SCHEDULER_H
