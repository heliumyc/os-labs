//
// Created by CONG YU on 2019/10/28.
//

#include "Scheduler.h"
#include "Schedulers/FCFS_Scheduler.h"
#include "Schedulers/LCFS_Scheduler.h"
#include "Schedulers/SRTF_Scheduler.h"
#include "Schedulers/RoundRobin_Scheduler.h"
#include "Schedulers/Priority_Scheduler.h"
#include "Schedulers/Preemptive_Priority_Scheduler.h"

Scheduler *SchedulerFactory::CreateScheduler(SchedulerEnum type) {
    Scheduler* createdScheduler = nullptr;
    switch (type) {
        case SchedulerEnum::FCFS: return new FCFS_Scheduler();
        case SchedulerEnum::LCFS: return new LCFS_Scheduler();
        case SchedulerEnum::SRTF: return new SRTF_SCheduler();
        case SchedulerEnum::RR:   return new RounRobin_Scheduler();
        case SchedulerEnum::PRIO: return new Priority_Scheduler();
        case SchedulerEnum::PREPRIO: return new Preemptive_Priority_Scheduler();
        default: return nullptr;
    }
}

