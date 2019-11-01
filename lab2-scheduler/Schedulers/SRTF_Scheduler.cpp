//
// Created by CONG YU on 2019/10/28.
//

#include "SRTF_Scheduler.h"

void SRTF_SCheduler::AddProcess(Process *p) {
    if (this->run_queue.empty()) {
        this->run_queue.push_back(p);
    } else {
        for (auto it = this->run_queue.begin(); it != this->run_queue.end(); ++it) {
            if ((*it)->remain_cpu_time > p->remain_cpu_time) {
                this->run_queue.insert(it, p);
                break;
            }
        }
    }
}

Process *SRTF_SCheduler::GetNextProcess() {
    Process* process = nullptr;
    if (!this->run_queue.empty()) {
        process = this->run_queue.front();
        this->run_queue.pop_front();
    }
    return process;
}

void SRTF_SCheduler::TryPreempt(Process *p, int curtime) {

}
