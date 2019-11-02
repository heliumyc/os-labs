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

Scheduler *SchedulerFactory::CreateScheduler(SchedulerEnum type, int quantum, int max_priority) {
    Scheduler* createdScheduler = nullptr;
    switch (type) {
        case SchedulerEnum::FCFS: createdScheduler = new FCFS_Scheduler(); break;
        case SchedulerEnum::LCFS: createdScheduler = new LCFS_Scheduler(); break;
        case SchedulerEnum::SRTF: createdScheduler = new SRTF_SCheduler(); break;
        case SchedulerEnum::RR:   createdScheduler = new RounRobin_Scheduler(); break;
        case SchedulerEnum::PRIO: createdScheduler = new Priority_Scheduler(max_priority); break;
        case SchedulerEnum::PREPRIO: createdScheduler = new Preemptive_Priority_Scheduler(max_priority); break;
    }
    return createdScheduler;
}

void Scheduler::setQuantum(int quantum) {
    this->quantum = quantum;
}

void Scheduler::setMaxPriority(int max_priority) {
    this->max_priority = max_priority;
}

std::string SchedulerEnumToString(SchedulerEnum s) {
    std::string str;
    switch (s) {
        case SchedulerEnum::FCFS: str = "FCFS"; break;
        case SchedulerEnum::LCFS: str = "LCFS"; break;
        case SchedulerEnum::SRTF: str = "SRTF"; break;
        case SchedulerEnum::RR: str = "RR"; break;
        case SchedulerEnum::PRIO: str = "PRIO"; break;
        case SchedulerEnum ::PREPRIO: str = "PREPRIO"; break;
    }
    return str;
}
