//
// Created by CONG YU on 2019/10/28.
//

#ifndef LAB2_SCHEDULER_LCLS_SCHEDULER_H
#define LAB2_SCHEDULER_LCLS_SCHEDULER_H


#include "../Scheduler.h"

class LCLS_Scheduler: public Scheduler {
    void addProcess(Process* p);
    Process* getNext();
    void testPreempt(Process* p, int curtime);
};


#endif //LAB2_SCHEDULER_LCLS_SCHEDULER_H
