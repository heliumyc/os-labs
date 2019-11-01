//
// Created by CONG YU on 2019/10/28.
//

#include "LCFS_Scheduler.h"

void LCFS_Scheduler::TryPreempt(Process *p, int curtime) {

}

Process *LCFS_Scheduler::GetNextProcess() {
    Process* process = nullptr;
    if (!this->run_queue.empty()) {
        process = run_queue.front();
        run_queue.pop_front();
    }
    return process;
}

void LCFS_Scheduler::AddProcess(Process *p) {
    this->run_queue.push_front(p);
}
