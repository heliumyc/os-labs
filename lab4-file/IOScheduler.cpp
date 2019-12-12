//
// Created by CONG YU on 2019/12/10.
//

#include "IOScheduler.h"

#include <memory>
#include <iostream>
#include "IO_Schedulers/FIFO_IOScheduler.h"

std::unique_ptr<IOScheduler> IOSchedulerFactory::CreateScheduler(IOSchedType io_sched_type) {
    switch (io_sched_type) {
        case IOSchedType::FIFO:  return std::make_unique<FIFO_IOScheduler>(); break;
//        case IOSchedType::CLOOK: sched = new CLOOK_IOScheduler(); break;
//        case IOSchedType::SSTF:  sched = new SSTF_IOScheduler(); break;;
//        case IOSchedType::FLOOK: sched = new FLOOK_IOScheduler(); break;;
//        case IOSchedType::LOOK:  sched = new LOOK_IOScheduler(); break;;
        default:
            return std::make_unique<FIFO_IOScheduler>();
    }
}

void IOScheduler::IncrementTime() {
    time++;
}

bool IOScheduler::IsActive() {
    return active_io == nullptr;
}

bool IOScheduler::IsCompleted() {
    return active_io->timestamp == time;
}

void IOScheduler::ClearActive() {
    active_io.reset();
}

