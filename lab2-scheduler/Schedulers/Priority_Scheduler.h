//
// Created by CONG YU on 2019/10/28.
//

#ifndef LAB2_SCHEDULER_PRIORITY_SCHEDULER_H
#define LAB2_SCHEDULER_PRIORITY_SCHEDULER_H


#include <queue>
#include "../Scheduler.h"

class Priority_Scheduler: public Scheduler {
    void AddProcess(Process* p) override;
    Process* GetNextProcess() override;
    void TryPreempt(Process* p, int cur_time) override;

public:
//    std::queue<Process*>** active_queue;
//    std::queue<Process*>** expired_queue;

/// you dont have to store the pointer of the vector to swap
/// because c++ has built-in vector::swap do the exactly the same!!!!!
    std::vector<std::queue<Process*>*> active_queue;
    std::vector<std::queue<Process*>*> expired_queue;

    explicit Priority_Scheduler(int max_priority);
};

#endif //LAB2_SCHEDULER_PRIORITY_SCHEDULER_H
