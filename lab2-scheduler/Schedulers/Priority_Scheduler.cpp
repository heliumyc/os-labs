//
// Created by CONG YU on 2019/10/28.
//

#include <numeric>
#include "Priority_Scheduler.h"

Priority_Scheduler::Priority_Scheduler(int _max_priority) {
    max_priority = _max_priority;
    assert(max_priority > 0);
    // you can do this, pointer to the array of pointers, wow!
//    active_queue = new std::queue<Process*>*[max_priority];
//    expired_queue = new std::queue<Process*>*[max_priority];
    active_queue.resize(max_priority);
    expired_queue.resize(max_priority);

    for (int i = 0; i < max_priority; ++i) {
        active_queue[i] = new std::queue<Process*>;
        expired_queue[i] = new std::queue<Process*>;
    }
}

void Priority_Scheduler::AddProcess(Process *p) {
    if (p->dynamic_priority < 0) { // or rather == -1
        p->dynamic_priority = p->static_priority - 1;
        expired_queue[p->dynamic_priority]->push(p);
    } else {
        active_queue[p->dynamic_priority]->push(p);
    }
}

Process *Priority_Scheduler::GetNextProcess() {

    // functional is awesome!
    bool nonactive = std::accumulate(active_queue.rbegin(), active_queue.rend(), true, [](bool acc, std::queue<Process*>* q){
        return acc && q->empty(); // move makes no difference here for primitive type like bool and int
    });

    if (nonactive) {
        active_queue.swap(expired_queue); // this swap is constant time
    }

    Process* p = nullptr;
    auto it = active_queue.rbegin();
    while (it != active_queue.rend()) {
        if ((*it)->empty()) {
            it++;
        } else {
            p = (*it)->front();
            (*it)->pop();
            break;
        }
    }
    return p;
}

void Priority_Scheduler::TryPreempt(Process *p, int cur_time) {
    p->dynamic_priority--;
    AddProcess(p);
}

