//
// Created by CONG YU on 2019/10/28.
//

#include "Scheduler.h"

Scheduler *SchedulerFactory::createScheduler(scheduler_t type) {
    Scheduler* createdScheduler = nullptr;
    switch (type) {
        case FCFS: return new FCFS_Scheduler();
        case LCLS: return new LCLS_Scheduler();
        case SRTF: return new SRTF_SCheduler();
        case RR:   return new RounRobin_Scheduler();
        case PRIO: return new Priority_Scheduler();
        case PREPRIO: return new Preemptive_Priority_Scheduler();
        default: return nullptr;
    }
}

