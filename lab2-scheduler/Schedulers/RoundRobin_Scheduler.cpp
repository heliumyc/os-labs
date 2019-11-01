//
// Created by CONG YU on 2019/10/28.
//

#include "RoundRobin_Scheduler.h"

void RounRobin_Scheduler::AddProcess(Process *p) {
    this->run_queue.push_back(p);
}

Process *RounRobin_Scheduler::GetNextProcess() {
    Process* process = nullptr;
    if (!this->run_queue.empty()) {
        process = run_queue.front();
        run_queue.pop_front();
    }
    return process;
}

void RounRobin_Scheduler::TryPreempt(Process *p, int curtime) {
    AddProcess(p);
}
