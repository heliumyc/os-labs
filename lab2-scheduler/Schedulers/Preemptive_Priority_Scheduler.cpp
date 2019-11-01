//
// Created by CONG YU on 2019/10/28.
//

#include "Preemptive_Priority_Scheduler.h"

void Preemptive_Priority_Scheduler::AddProcess(Process *p) {

}

Process *Preemptive_Priority_Scheduler::GetNextProcess() {
    return nullptr;
}

void Preemptive_Priority_Scheduler::TryPreempt(Process *p, int curtime) {

}

Preemptive_Priority_Scheduler::Preemptive_Priority_Scheduler(int max_priority) {
    this->max_priority = max_priority;
}
