//
// Created by CONG YU on 2019/10/28.
//

#include "FCFS_Scheduler.h"

void FCFS_Scheduler::AddProcess(Process *p) {
    run_queue.push_back(p);
}

Process *FCFS_Scheduler::GetNextProcess() {
    Process* process = nullptr;
    if (!run_queue.empty()) {
        process = run_queue.front();
        run_queue.pop_front();
    }
    return process;
}

void FCFS_Scheduler::TryPreempt(Process *p, int curtime) {

}
